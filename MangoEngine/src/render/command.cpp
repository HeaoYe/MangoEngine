#include "MangoEngine/render/command.hpp"
#include "MangoEngine/render/render.hpp"

namespace MangoEngine {
    QuadInstance::QuadInstance(glm::vec3 pos, glm::vec2 size, glm::vec4 color, int texture_slot) : color(color), texture_slot(texture_slot) {
        pos.x = pos.x / render_system->get_viewport().width * 2 - 1;
        pos.y = pos.y / render_system->get_viewport().height * 2 - 1;
        size.x /= render_system->get_viewport().width;
        size.y /= render_system->get_viewport().height;
        this->pos = pos;
        this->size = size;
    }

    RenderCommand::RenderCommand(RenderSystem &render_system) : vertex_buffer(render_system.get_context().get_resource_manager_reference().create_vertex_buffer(sizeof(QuadInstance))) {
        _current_vertex_buffer_offset = 0;
        _current_texture_slot = 0;
        quads.reserve(render_command_max_quad_buffer_size);
    }

    RenderCommand::~RenderCommand() {
        quads.clear();
    }

    void RenderCommand::draw_quad(glm::vec3 pos, glm::vec2 size, glm::vec4 color) {
        quads.emplace_back(pos, size, color, -1);
        if (quads.size() == render_command_max_quad_buffer_size) {
            flush();
        }
    }

    void RenderCommand::flush() {
        vertex_buffer.write_data(quads.data(), quads.size(), _current_vertex_buffer_offset);
        _current_vertex_buffer_offset += quads.size();
        quads.clear();
        _current_texture_slot = 0;
    }

    void RenderCommand::render(MangoRHI::Command &command) {
        if (quads.size() > 0) {
            flush();
        }
        command.bind_vertex_buffer(vertex_buffer, 0);
        command.draw_instances(6, _current_vertex_buffer_offset, 0, 0);
        _current_vertex_buffer_offset = 0;
    }
}
