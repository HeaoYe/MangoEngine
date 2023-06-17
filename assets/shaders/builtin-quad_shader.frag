#version 450

layout(location = 0) in vec4 frag_color;
layout(location = 1) in vec2 frag_uv;
layout(location = 2) flat in int frag_texture_slot;

layout(location = 0) out vec4 out_color;

layout(set = 1, binding = 0) uniform sampler2D textures[32];

void main() {
    if (0 <= frag_texture_slot && frag_texture_slot < 32) {
        out_color = texture(textures[frag_texture_slot], frag_uv) * frag_color;
    }
    else {
        out_color = frag_color;
    }
}
