#pragma once

#include "../commons.hpp"
#include "texture.hpp"
#include "camera/camera.hpp"

namespace MangoEngine {
    struct QuadInstance {
        glm::vec3 pos;
        glm::vec2 size;
        f32 rotate;
        glm::vec4 color;
        int texture_slot;
        QuadInstance(glm::vec3 pos, glm::vec2 size, f32 rotate, glm::vec4 color, int texture_slot);
    };

    class RenderSystem;
    constexpr u32 render_command_max_quad_buffer_size = 2048;
    constexpr u32 render_command_max_texture_slot_count = 32;  // same in builtin-quad_shader.frag (set = 1, binding = 0)
    constexpr u32 render_command_max_quad_instance_per_frame = 1024 * 1024;

    class RenderCommand {
    public:
        RenderCommand(RenderSystem &render_system, Bool is_support_alpha);
        ~RenderCommand();

        void begin_scene(std::shared_ptr<Camera> camera);
        void end_scene();

        void draw_quad(glm::vec3 pos, glm::vec2 size, f32 rotate, glm::vec4 color, std::weak_ptr<Texture> texture);
        void draw_quad(glm::vec3 pos, glm::vec2 size, f32 rotate, glm::vec3 color, std::weak_ptr<Texture> texture) { draw_quad(pos, size, rotate, { color.r, color.g, color.b, 1.0f }, texture); }
        void draw_quad(glm::vec2 pos, glm::vec2 size, f32 rotate, glm::vec4 color, std::weak_ptr<Texture> texture) { draw_quad({ pos.x, pos.y, 0.0f }, size, rotate, color, texture); }
        void draw_quad(glm::vec2 pos, glm::vec2 size, f32 rotate, glm::vec3 color, std::weak_ptr<Texture> texture) { draw_quad({ pos.x, pos.y, 0.0f }, size, rotate, { color.r, color.g, color.b, 1.0f }, texture); }
        void draw_quad(glm::vec3 pos, f32 size, f32 rotate, glm::vec4 color, std::weak_ptr<Texture> texture) { draw_quad(pos, { size, size }, rotate, color, texture); }
        void draw_quad(glm::vec3 pos, f32 size, f32 rotate, glm::vec3 color, std::weak_ptr<Texture> texture) { draw_quad(pos, { size, size }, rotate, { color.r, color.g, color.b, 1.0f }, texture); }
        void draw_quad(glm::vec2 pos, f32 size, f32 rotate, glm::vec4 color, std::weak_ptr<Texture> texture) { draw_quad({ pos.x, pos.y, 0.0f }, { size, size }, rotate, color, texture); }
        void draw_quad(glm::vec2 pos, f32 size, f32 rotate, glm::vec3 color, std::weak_ptr<Texture> texture) { draw_quad({ pos.x, pos.y, 0.0f }, { size, size }, rotate, { color.r, color.g, color.b, 1.0f }, texture); }

        void draw_quad(glm::vec3 pos, glm::vec2 size, f32 rotate, std::weak_ptr<Texture> texture) { draw_quad(pos, size, 0.0f, { 0.5f, 0.5f, 0.5f, 1.0f }, texture); }
        void draw_quad(glm::vec2 pos, glm::vec2 size, f32 rotate, std::weak_ptr<Texture> texture) { draw_quad({ pos.x, pos.y, 0.0f }, size, 0.0f, { 0.5f, 0.5f, 0.5f, 1.0f }, texture); }
        void draw_quad(glm::vec3 pos, f32 size, f32 rotate, std::weak_ptr<Texture> texture) { draw_quad(pos, { size, size }, 0.0f, { 0.5f, 0.5f, 0.5f, 1.0f }, texture); }
        void draw_quad(glm::vec2 pos, f32 size, f32 rotate, std::weak_ptr<Texture> texture) { draw_quad({ pos.x, pos.y, 0.0f }, { size, size }, 0.0f, { 0.5f, 0.5f, 0.5f, 1.0f }, texture); }

        void draw_quad(glm::vec3 pos, glm::vec2 size, glm::vec4 color, std::weak_ptr<Texture> texture) { draw_quad(pos, size, 0.0f, color); }
        void draw_quad(glm::vec3 pos, glm::vec2 size, glm::vec3 color, std::weak_ptr<Texture> texture) { draw_quad(pos, size, 0.0f, { color.r, color.g, color.b, 1.0f }, texture); }
        void draw_quad(glm::vec2 pos, glm::vec2 size, glm::vec4 color, std::weak_ptr<Texture> texture) { draw_quad({ pos.x, pos.y, 0.0f }, size, 0.0f, color, texture); }
        void draw_quad(glm::vec2 pos, glm::vec2 size, glm::vec3 color, std::weak_ptr<Texture> texture) { draw_quad({ pos.x, pos.y, 0.0f }, size, 0.0f, { color.r, color.g, color.b, 1.0f }, texture); }
        void draw_quad(glm::vec3 pos, f32 size, glm::vec4 color, std::weak_ptr<Texture> texture) { draw_quad(pos, { size, size }, 0.0f, color, texture); }
        void draw_quad(glm::vec3 pos, f32 size, glm::vec3 color, std::weak_ptr<Texture> texture) { draw_quad(pos, { size, size }, 0.0f, { color.r, color.g, color.b, 1.0f }, texture); }
        void draw_quad(glm::vec2 pos, f32 size, glm::vec4 color, std::weak_ptr<Texture> texture) { draw_quad({ pos.x, pos.y, 0.0f }, { size, size }, 0.0f, color, texture); }
        void draw_quad(glm::vec2 pos, f32 size, glm::vec3 color, std::weak_ptr<Texture> texture) { draw_quad({ pos.x, pos.y, 0.0f }, { size, size }, 0.0f, { color.r, color.g, color.b, 1.0f }, texture); }

        void draw_quad(glm::vec3 pos, glm::vec2 size, std::weak_ptr<Texture> texture) { draw_quad(pos, size, 0.0f, { 0.5f, 0.5f, 0.5f, 1.0f }, texture); }
        void draw_quad(glm::vec2 pos, glm::vec2 size, std::weak_ptr<Texture> texture) { draw_quad({ pos.x, pos.y, 0.0f }, size, 0.0f, { 0.5f, 0.5f, 0.5f, 1.0f }, texture); }
        void draw_quad(glm::vec3 pos, f32 size, std::weak_ptr<Texture> texture) { draw_quad(pos, { size, size }, 0.0f, { 0.5f, 0.5f, 0.5f, 1.0f }, texture); }
        void draw_quad(glm::vec2 pos, f32 size, std::weak_ptr<Texture> texture) { draw_quad({ pos.x, pos.y, 0.0f }, { size, size }, 0.0f, { 0.5f, 0.5f, 0.5f, 1.0f }, texture); }

        void draw_quad(glm::vec3 pos, glm::vec2 size, f32 rotate, glm::vec4 color);
        void draw_quad(glm::vec3 pos, glm::vec2 size, f32 rotate, glm::vec3 color) { draw_quad(pos, size, rotate, { color.r, color.g, color.b, 1.0f }); }
        void draw_quad(glm::vec2 pos, glm::vec2 size, f32 rotate, glm::vec4 color) { draw_quad({ pos.x, pos.y, 0.0f }, size, rotate, color); }
        void draw_quad(glm::vec2 pos, glm::vec2 size, f32 rotate, glm::vec3 color) { draw_quad({ pos.x, pos.y, 0.0f }, size, rotate, { color.r, color.g, color.b, 1.0f }); }
        void draw_quad(glm::vec3 pos, f32 size, f32 rotate, glm::vec4 color) { draw_quad(pos, { size, size }, rotate, color); }
        void draw_quad(glm::vec3 pos, f32 size, f32 rotate, glm::vec3 color) { draw_quad(pos, { size, size }, rotate, { color.r, color.g, color.b, 1.0f }); }
        void draw_quad(glm::vec2 pos, f32 size, f32 rotate, glm::vec4 color) { draw_quad({ pos.x, pos.y, 0.0f }, { size, size }, rotate, color); }
        void draw_quad(glm::vec2 pos, f32 size, f32 rotate, glm::vec3 color) { draw_quad({ pos.x, pos.y, 0.0f }, { size, size }, rotate, { color.r, color.g, color.b, 1.0f }); }

        void draw_quad(glm::vec3 pos, glm::vec2 size, f32 rotate) { draw_quad(pos, size, 0.0f, { 0.5f, 0.5f, 0.5f, 1.0f }); }
        void draw_quad(glm::vec2 pos, glm::vec2 size, f32 rotate) { draw_quad({ pos.x, pos.y, 0.0f }, size, 0.0f, { 0.5f, 0.5f, 0.5f, 1.0f }); }
        void draw_quad(glm::vec3 pos, f32 size, f32 rotate) { draw_quad(pos, { size, size }, 0.0f, { 0.5f, 0.5f, 0.5f, 1.0f }); }
        void draw_quad(glm::vec2 pos, f32 size, f32 rotate) { draw_quad({ pos.x, pos.y, 0.0f }, { size, size }, 0.0f, { 0.5f, 0.5f, 0.5f, 1.0f }); }

        void draw_quad(glm::vec3 pos, glm::vec2 size, glm::vec4 color) { draw_quad(pos, size, 0.0f, color); }
        void draw_quad(glm::vec3 pos, glm::vec2 size, glm::vec3 color) { draw_quad(pos, size, 0.0f, { color.r, color.g, color.b, 1.0f }); }
        void draw_quad(glm::vec2 pos, glm::vec2 size, glm::vec4 color) { draw_quad({ pos.x, pos.y, 0.0f }, size, 0.0f, color); }
        void draw_quad(glm::vec2 pos, glm::vec2 size, glm::vec3 color) { draw_quad({ pos.x, pos.y, 0.0f }, size, 0.0f, { color.r, color.g, color.b, 1.0f }); }
        void draw_quad(glm::vec3 pos, f32 size, glm::vec4 color) { draw_quad(pos, { size, size }, 0.0f, color); }
        void draw_quad(glm::vec3 pos, f32 size, glm::vec3 color) { draw_quad(pos, { size, size }, 0.0f, { color.r, color.g, color.b, 1.0f }); }
        void draw_quad(glm::vec2 pos, f32 size, glm::vec4 color) { draw_quad({ pos.x, pos.y, 0.0f }, { size, size }, 0.0f, color); }
        void draw_quad(glm::vec2 pos, f32 size, glm::vec3 color) { draw_quad({ pos.x, pos.y, 0.0f }, { size, size }, 0.0f, { color.r, color.g, color.b, 1.0f }); }

        void draw_quad(glm::vec3 pos, glm::vec2 size) { draw_quad(pos, size, 0.0f, { 0.5f, 0.5f, 0.5f, 1.0f }); }
        void draw_quad(glm::vec2 pos, glm::vec2 size) { draw_quad({ pos.x, pos.y, 0.0f }, size, 0.0f, { 0.5f, 0.5f, 0.5f, 1.0f }); }
        void draw_quad(glm::vec3 pos, f32 size) { draw_quad(pos, { size, size }, 0.0f, { 0.5f, 0.5f, 0.5f, 1.0f }); }
        void draw_quad(glm::vec2 pos, f32 size) { draw_quad({ pos.x, pos.y, 0.0f }, { size, size }, 0.0f, { 0.5f, 0.5f, 0.5f, 1.0f }); }

    private:
        void flush();
        void allocate_descriptor_set();

        std::unique_ptr<MangoRHI::ShaderProgram> quad_shader_program;
        std::unordered_map<u32, std::pair<u32, u32>> texture_cache;
        std::unique_ptr<MangoRHI::Texture> default_texture;
        std::unique_ptr<MangoRHI::DescriptorSet> default_descriptor_set;
        std::unique_ptr<MangoRHI::VertexBuffer> vertex_buffer;
        u32 _vertex_buffer_offset;

        std::vector<QuadInstance> quads;
        std::unique_ptr<MangoRHI::DescriptorSet> camera_uniform_descriptor_set;

        std::vector<std::vector<QuadInstance>> texture_quads;
        std::vector<std::unique_ptr<MangoRHI::DescriptorSet>> descriptor_sets;
        i32 _descriptor_set;
        u32 _texture_slot;
    };
}
