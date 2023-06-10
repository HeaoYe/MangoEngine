#pragma once

#include "../commons.hpp"

namespace MangoEngine {
    struct QuadInstance {
        glm::vec3 pos;
        glm::vec2 size;
        glm::vec4 color;
        int texture_slot;
        QuadInstance(glm::vec3 pos, glm::vec2 size, glm::vec4 color, int texture_slot);
    };

    class RenderSystem;
    constexpr u32 render_command_max_quad_buffer_size = 256;

    class RenderCommand {
    public:
        RenderCommand(RenderSystem &render_system);
        ~RenderCommand();

        void flush();
        void render(MangoRHI::Command &command);

        void draw_quad(glm::vec3 pos, glm::vec2 size, glm::vec4 color);
        void draw_quad(glm::vec3 pos, f32 size, glm::vec4 color) { draw_quad(pos, { size, size }, color); }
        void draw_quad(glm::vec3 pos, glm::vec2 size, glm::vec3 color) { draw_quad(pos, size, { color.r, color.g, color.b, 1.0f }); }
        void draw_quad(glm::vec3 pos, f32 size, glm::vec3 color) { draw_quad(pos, { size, size }, { color.r, color.g, color.b, 1.0f }); }
        void draw_quad(glm::vec2 pos, glm::vec2 size, glm::vec4 color) { draw_quad({ pos.x, pos.y, 0.5 }, size, color); }
        void draw_quad(glm::vec2 pos, f32 size, glm::vec4 color) { draw_quad({ pos.x, pos.y, 0.5 }, { size, size }, color); }
        void draw_quad(glm::vec2 pos, glm::vec2 size, glm::vec3 color) { draw_quad({ pos.x, pos.y, 0.5 }, size, { color.r, color.g, color.b, 1.0f }); }
        void draw_quad(glm::vec2 pos, f32 size, glm::vec3 color) { draw_quad({ pos.x, pos.y, 0.5 }, { size, size }, { color.r, color.g, color.b, 1.0f }); }

        void draw_quad(glm::vec3 pos, glm::vec2 size) { draw_quad(pos, size, { 0.5f, 0.5f, 0.5f, 1.0f }); }
        void draw_quad(glm::vec3 pos, f32 size) { draw_quad(pos, { size, size }, { 0.5f, 0.5f, 0.5f, 1.0f }); }
        void draw_quad(glm::vec2 pos, glm::vec2 size) { draw_quad({ pos.x, pos.y, 0.5 }, size, { 0.5f, 0.5f, 0.5f, 1.0f }); }
        void draw_quad(glm::vec2 pos, f32 size) { draw_quad({ pos.x, pos.y, 0.5 }, { size, size }, { 0.5f, 0.5f, 0.5f, 1.0f }); }

    private:
        std::vector<QuadInstance> quads;
        u32 _current_vertex_buffer_offset;
        u32 _current_texture_slot;
        MangoRHI::VertexBuffer &vertex_buffer;
    };
}
