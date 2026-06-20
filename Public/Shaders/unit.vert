#version 450

// Input from compute shader output buffer (world space)
layout (location = 0) in vec4 a_Position;     // Clip space position from compute shader
layout (location = 1) in vec2 a_TexCoords;    // Texture coordinates
layout (location = 2) in uint a_LightingEmit; // Lighting emit value
layout (location = 3) in uint a_TransparencyLevel; // Transparency level (0-255)
layout (location = 4) in uint a_FaceIndex;    // Face index (0-5)
layout (location = 5) in vec3 a_WorldPos;     // World space position
layout (location = 6) in vec3 a_Albedo;        // PBR albedo
layout (location = 7) in float a_Metallic;     // PBR metallic
layout (location = 8) in float a_Roughness;    // PBR roughness
layout (location = 9) in vec3 a_Tangent;       // TBN tangent for normal mapping
layout (location = 10) in vec3 a_Bitangent;    // TBN bitangent for normal mapping

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
layout (location = 5) flat out vec3 v_Albedo;
layout (location = 6) flat out float v_Metallic;
layout (location = 7) flat out float v_Roughness;
layout (location = 8) flat out mat3 v_TBN;

void main() {
    // Pass through clip space position from compute shader
    gl_Position = a_Position;

    // Transform TBN vectors to world space
    vec3 T = normalize(mat3(pc.model) * a_Tangent);
    vec3 B = normalize(mat3(pc.model) * a_Bitangent);
    vec3 N = normalize(cross(B, T));
    
    // Re-orthogonalize TBN matrix for numerical stability
    N = normalize(N);
    T = normalize(T - dot(T, N) * N);
    B = cross(T, N);
    
    // Pass through other data
    v_WorldPos = a_WorldPos;
    v_TexCoords = a_TexCoords;
    v_LightingEmit = a_LightingEmit;
    v_TransparencyLevel = a_TransparencyLevel;
    v_FaceIndex = a_FaceIndex;
    v_Albedo = a_Albedo;
    v_Metallic = a_Metallic;
    v_Roughness = a_Roughness;
    v_TBN = mat3(T, B, N);
}