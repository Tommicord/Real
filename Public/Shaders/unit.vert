#version 450

// Input from compute shader output buffer
layout (location = 0) in vec4 a_Position;     // Clip space position
layout (location = 1) in vec3 a_WorldPos;     // World space position
layout (location = 2) in vec2 a_TexCoords;    // Texture coordinates
layout (location = 3) in uint a_LightingEmit; // Lighting emit value
layout (location = 4) in uint a_TransparencyLevel; // Transparency level (0-255)
layout (location = 5) in uint a_FaceIndex;    // Face index (0-5)
layout (location = 6) in vec3 a_Albedo;      // PBR albedo
layout (location = 7) in float a_Metallic;   // PBR metallic
layout (location = 8) in float a_Roughness;  // PBR roughness

// Output to fragment shader
layout (location = 0) out vec3 v_WorldPos;
layout (location = 1) out vec2 v_TexCoords;
layout (location = 2) out uint v_LightingEmit;
layout (location = 3) out uint v_TransparencyLevel;
layout (location = 4) out uint v_FaceIndex;
layout (location = 5) out vec3 v_Albedo;
layout (location = 6) out float v_Metallic;
layout (location = 7) out float v_Roughness;

void main() {
    // Pass through data from compute shader output
    gl_Position = a_Position;
    v_WorldPos = a_WorldPos;
    v_TexCoords = a_TexCoords;
    v_LightingEmit = a_LightingEmit;
    v_TransparencyLevel = a_TransparencyLevel;
    v_FaceIndex = a_FaceIndex;
    v_Albedo = a_Albedo;
    v_Metallic = a_Metallic;
    v_Roughness = a_Roughness;
}