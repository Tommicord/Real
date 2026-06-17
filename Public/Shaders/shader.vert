#version 450

layout(push_constant) uniform PushConstants {
    mat4 model;
    mat4 view;
    mat4 projection;
} pc;

// Test triangle with PVM matrices
vec3 positions[3] = vec3[](
    vec3(0.0, -0.5, 0.0),
    vec3(0.5, 0.5, 0.0),
    vec3(-0.5, 0.5, 0.0)
);

vec3 colors[3] = vec3[](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);

layout(location = 0) out vec3 fragColor;

void main() {
    vec3 pos = positions[gl_VertexIndex];
    gl_Position = pc.projection * pc.view * pc.model * vec4(pos, 1.0);
    fragColor = colors[gl_VertexIndex];
}