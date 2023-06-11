#version 450

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec2 in_size;
layout(location = 2) in float in_rotate;
layout(location = 3) in vec4 in_color;
layout(location = 4) in int texture_slot;

layout(binding = 0) uniform ViewProject {
    mat4 view;
    mat4 project;
} vp;

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
    float s = sin(in_rotate);
    float c = cos(in_rotate);

    vec2 quad_pos = quad[gl_VertexIndex / 3 + gl_VertexIndex % 3];
    gl_Position = vp.project * vp.view * vec4(mat2(c, s, -s, c) * quad_pos * in_size + in_pos.xy, in_pos.z, 1);
    frag_uv = vec2(quad_pos.x + 0.5, 0.5 - quad_pos.y);
    frag_color = in_color;
    frag_texture_slot = texture_slot;
}
