#pragma once

#include "../commons.hpp"
#include "texture.hpp"

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

    class RenderCommand {
    public:
        RenderCommand(RenderSystem &render_system);
        ~RenderCommand();

        void flush();
        void render(MangoRHI::Command &command);

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
        std::vector<QuadInstance> quads;
        u32 _current_vertex_buffer_offset;
        u32 _current_texture_slot;
        std::unordered_map<u32, u32> uuid2texture_slot;
        std::unique_ptr<MangoRHI::VertexBuffer> vertex_buffer;
    };
}
