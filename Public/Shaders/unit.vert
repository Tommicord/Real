#version 450

// Input from vertex buffer (local space)
layout (location = 0) in vec3 a_Position;     // Local space position
layout (location = 1) in vec2 a_TexCoords;    // Texture coordinates
layout (location = 2) in uint a_LightingEmit; // Lighting emit value
layout (location = 3) in uint a_TransparencyLevel; // Transparency level (0-255)
layout (location = 4) in uint a_FaceIndex;    // Face index (0-5)
layout (location = 5) in vec4 a_PolRight;     // Polygon fence right offsets
layout (location = 6) in vec4 a_PolLeft;      // Polygon fence left offsets
layout (location = 7) in vec3 a_Albedo;      // PBR albedo
layout (location = 8) in float a_Metallic;   // PBR metallic
layout (location = 9) in float a_Roughness;  // PBR roughness

// Push constants for camera matrices
layout(push_constant) uniform PushConstants {
    mat4 model;
    mat4 view;
    mat4 projection;
} pc;

// Output to fragment shader
layout (location = 0) out vec3 v_WorldPos;
layout (location = 1) smooth out vec2 v_TexCoords;
layout (location = 2) flat out uint v_LightingEmit;
layout (location = 3) flat out uint v_TransparencyLevel;
layout (location = 4) flat out uint v_FaceIndex;
layout (location = 5) out vec3 v_Albedo;
layout (location = 6) out float v_Metallic;
layout (location = 7) out float v_Roughness;

void main() {
    // Transform local space position to world space
    vec4 worldPos = pc.model * vec4(a_Position, 1.0);
    
    // Transform world space position to clip space
    gl_Position = pc.projection * pc.view * worldPos;
    
    // Pass through other data
    v_WorldPos = worldPos.xyz;
    v_TexCoords = a_TexCoords;
    v_LightingEmit = a_LightingEmit;
    v_TransparencyLevel = a_TransparencyLevel;
    v_FaceIndex = a_FaceIndex;
    v_Albedo = a_Albedo;
    v_Metallic = a_Metallic;
    v_Roughness = a_Roughness;
}