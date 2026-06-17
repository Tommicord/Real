#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
} ubo;

vec2 positions[3] = vec2[](
    vec2(0.0, -1.0),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.4)
);

vec3 colors[3] = vec3[](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);

layout(location = 0) out vec3 fragColor;

void main() {
    vec2 pos = positions[gl_VertexIndex];
    vec4 worldPos = ubo.model * vec4(pos, 0.0, 1.0);
    vec4 viewPos = ubo.view * worldPos;
    gl_Position = ubo.projection * viewPos;
    fragColor = colors[gl_VertexIndex];
}