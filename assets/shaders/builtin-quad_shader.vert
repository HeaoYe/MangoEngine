#version 450

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec2 in_size;
layout(location = 2) in vec4 in_color;
layout(location = 3) in int texture_slot;

layout(location = 0) out vec4 frag_color;
layout(location = 1) out vec2 frag_uv;
layout(location = 2) flat out int frag_texture_slot;

vec2 quad[4] = vec2[](
    vec2(-0.5, -0.5),
    vec2(-0.5, 0.5),
    vec2(0.5, -0.5),
    vec2(0.5, 0.5)
);

void main() {
    vec2 quad_pos = quad[gl_VertexIndex / 3 + gl_VertexIndex % 3];
    gl_Position = vec4(quad_pos * in_size + in_pos.xy, in_pos.z, 1);
    frag_uv = quad_pos + 0.5;
    frag_color = in_color;
    frag_texture_slot = texture_slot;
}
