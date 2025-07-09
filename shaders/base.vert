#version 460 core

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_size;

layout(binding = 0) uniform Uniform {
    mat4 projection;
};

vec2 vertices[] = vec2[](
    vec2(0.0, 0.0),
    vec2(1.0, 0.0),
    vec2(1.0, 1.0),

    vec2(1.0, 1.0),
    vec2(0.0, 1.0),
    vec2(0.0, 0.0)
);

void main() {
    gl_Position = projection * vec4(vertices[gl_VertexIndex] * in_size + in_position, 0.0, 1.0);
}
