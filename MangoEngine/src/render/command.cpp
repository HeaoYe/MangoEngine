#include "MangoEngine/render/command.hpp"
#include "MangoEngine/render/render.hpp"

namespace MangoEngine {
    QuadInstance::QuadInstance(glm::vec3 pos, glm::vec2 size, f32 rotate, glm::vec4 color, int texture_slot) : pos(pos), size(size), rotate(rotate), color(color), texture_slot(texture_slot) {}

    RenderCommand::RenderCommand(RenderSystem &render_system, Bool is_support_alpha) {
        _vertex_buffer_offset = 0;
        _descriptor_set = -1;
        _texture_slot = render_command_max_texture_slot_count;
        quads.reserve(render_command_max_quad_buffer_size);
        auto &context = MangoRHI::get_context();
        auto &resource_factory = context.get_resource_factory_reference();
        vertex_buffer = std::move(resource_factory.create_vertex_buffer(sizeof(QuadInstance), render_command_max_quad_instance_per_frame));

        // if (is_support_alpha == MG_TRUE) {
        //     quad_shader_program = std::move(resource_factory.create_shader_program("main-alpha"));
        // } else {
        //     quad_shader_program = std::move(resource_factory.create_shader_program("main"));
        // }
        quad_shader_program = std::move(resource_factory.create_shader_program("main"));
        quad_shader_program->add_vertex_attribute(MangoRHI::VertexInputType::eFloat3, sizeof(glm::vec3));
        quad_shader_program->add_vertex_attribute(MangoRHI::VertexInputType::eFloat2, sizeof(glm::vec2));
        quad_shader_program->add_vertex_attribute(MangoRHI::VertexInputType::eFloat, sizeof(f32));
        quad_shader_program->add_vertex_attribute(MangoRHI::VertexInputType::eFloat4, sizeof(glm::vec4));
        quad_shader_program->add_vertex_attribute(MangoRHI::VertexInputType::eInt, sizeof(i32));
        quad_shader_program->add_vertex_binding(MangoRHI::VertexInputRate::ePerInstance);
        auto builtin_quad_vert_shader = resource_factory.create_shader("assets/shaders/builtin-quad_shader.vert.spv");
        auto builtin_quad_frag_shader = resource_factory.create_shader("assets/shaders/builtin-quad_shader.frag.spv");
        quad_shader_program->attach_vertex_shader(*builtin_quad_vert_shader, "main");
        quad_shader_program->attach_fragment_shader(*builtin_quad_frag_shader, "main");
        quad_shader_program->set_cull_mode(MangoRHI::CullMode::eNone);
        quad_shader_program->set_depth_test_enabled(MangoRHI::MG_TRUE);
        quad_shader_program->set_depth_compare_op(MangoRHI::DepthCompareOp::eLessOrEqual);
        if (is_support_alpha == MG_TRUE) {
            quad_shader_program->set_depth_write_enabled(MangoRHI::MG_FALSE);
        } else {
            quad_shader_program->set_depth_write_enabled(MangoRHI::MG_TRUE);
        }

        auto camera_uniform_layout = quad_shader_program->create_descriptor_set_layout("camera uniform layout");
        camera_uniform_layout.lock()->add_uniforms_descriptor(MangoRHI::DescriptorStage::eVertex, sizeof(glm::mat4) * 2, 1);
        auto quad_texture_layout = quad_shader_program->create_descriptor_set_layout("quad texture layout");
        quad_texture_layout.lock()->add_textures_descriptor(MangoRHI::DescriptorStage::eFragment, render_command_max_texture_slot_count);

        quad_shader_program->create();

        camera_uniform_descriptor_set = quad_shader_program->allocate_descriptor_set("camera uniform layout");
        camera_uniform_descriptor_set->update_all();

        default_texture = resource_factory.create_empty_texture();
        default_descriptor_set = quad_shader_program->allocate_descriptor_set("quad texture layout");
        for (u32 index = 0; index < render_command_max_texture_slot_count; index ++) {
            default_descriptor_set->set_texture(0, index, *default_texture);
        }
        default_descriptor_set->update_all();
    }

    RenderCommand::~RenderCommand() {
        quads.clear();
        texture_quads.clear();
        std::for_each(descriptor_sets.begin(), descriptor_sets.end(), [this](auto &descriptor_set) {
            quad_shader_program->free_descriptor_set(std::move(descriptor_set));
        });
        descriptor_sets.clear();
        default_texture.reset();
    }

    void RenderCommand::allocate_descriptor_set() {
        auto descriptor_set = quad_shader_program->allocate_descriptor_set("quad texture layout");
        for (u32 index = 0; index < render_command_max_texture_slot_count; index ++) {
            descriptor_set->set_texture(0, index, *default_texture);
        }
        descriptor_set->update_all();
        descriptor_sets.push_back(std::move(descriptor_set));
        texture_quads.emplace_back().reserve(render_command_max_quad_buffer_size);
    }

    void RenderCommand::draw_quad(glm::vec3 pos, glm::vec2 size, f32 rotate, glm::vec4 color, std::weak_ptr<Texture> texture) {
        u32 set, slot;
        if (texture_cache.find(texture.lock()->uuid) == texture_cache.end()) {
            if (_texture_slot >= render_command_max_texture_slot_count) {
                _descriptor_set ++;
                if (_descriptor_set >= descriptor_sets.size()) {
                    allocate_descriptor_set();
                }
                _texture_slot = 0;
            }
            set = _descriptor_set;
            slot = _texture_slot;
            texture_cache[texture.lock()->uuid] = std::make_pair(set, slot);
            descriptor_sets[set]->set_texture(0, slot, texture.lock()->get_texture_reference());
            _texture_slot ++;
        } else {
            auto &data = texture_cache[texture.lock()->uuid];
            set = data.first;
            slot = data.second;
        }
        texture_quads[set].emplace_back(pos, size, rotate, color, slot);
    }

    void RenderCommand::draw_quad(glm::vec3 pos, glm::vec2 size, f32 rotate, glm::vec4 color) {
        quads.emplace_back(pos, size, rotate, color, -1);
        if (quads.size() >= render_command_max_quad_buffer_size) {
            flush();
        }
    }

    void RenderCommand::flush() {
        vertex_buffer->write_data(quads.data(), quads.size(), _vertex_buffer_offset);
        _vertex_buffer_offset += quads.size();
        quads.clear();

        MANGO_ASSERT(_vertex_buffer_offset < render_command_max_quad_instance_per_frame)
    }

    void RenderCommand::begin_scene(std::shared_ptr<Camera> camera) {
        void *ptr = camera_uniform_descriptor_set->get_uniform_buffer_pointer(0, 0);
        memcpy(ptr, camera->get_datas_ptr(), sizeof(glm::mat4) * 2);
    }

    void RenderCommand::end_scene() {
        if (quads.size() > 0) {
            flush();
        }

        u32 instance_count = _vertex_buffer_offset;
        for (u32 index = 0; index < descriptor_sets.size(); index ++) {
            u32 quad_count = texture_quads[index].size();
            if (quad_count > 0) {
                descriptor_sets[index]->update_all();
                vertex_buffer->write_data(texture_quads[index].data(), quad_count, _vertex_buffer_offset);
                _vertex_buffer_offset += quad_count;
                MANGO_ASSERT(_vertex_buffer_offset < render_command_max_quad_instance_per_frame)
            }
        }
        auto &command = MangoRHI::get_context().get_current_command_reference();
        command.bind_vertex_buffer(*vertex_buffer, 0);
        command.bind_shader_program(*quad_shader_program);
        command.bind_descriptor_set(*camera_uniform_descriptor_set);
        _vertex_buffer_offset = instance_count;
        if (texture_quads.size() > 0) {
            for (u32 index = 0; index < descriptor_sets.size(); index ++) {
                command.bind_descriptor_set(*descriptor_sets[index]);
                u32 quad_count = texture_quads[index].size();
                if (quad_count > 0) {
                    texture_quads[index].clear();
                    command.draw_instances(6, quad_count, 0, _vertex_buffer_offset);
                    _vertex_buffer_offset += quad_count;
                }
            }
        } else {
            command.bind_descriptor_set(*default_descriptor_set);
        }
        if (instance_count > 0) {
            command.draw_instances(6, instance_count, 0, 0);
        }

        _vertex_buffer_offset = 0;
        _texture_slot = render_command_max_texture_slot_count;
        _descriptor_set = -1;
        texture_cache.clear();
    }
}
