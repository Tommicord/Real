#version 450

// Input from vertex shader (after compute shader culling)
layout (location = 0) in vec3 v_WorldPos;
layout (location = 1) smooth in vec2 v_TexCoords;
layout (location = 2) flat in uint v_LightingEmit;
layout (location = 3) flat in uint v_TransparencyLevel;
layout (location = 4) flat in uint v_FaceIndex;

// Output color
layout (location = 0) out vec4 outColor;

// The texture of the Unit
// The unit has 6 faces
layout (binding = 2) uniform sampler2D u_Texture[6];

// Lighting input (if using separate lighting pass)
layout (binding = 8) uniform sampler2D u_LightingTexture;

// Use separate lighting pass
layout(set = 0, binding = 9) uniform SettingsBlock {
    bool u_UseLightingPass;
} settings;

void main() {
    // Sample texture based on face index
    vec4 texColor = texture(u_Texture[v_FaceIndex], v_TexCoords);
    // Apply transparency
    float transparency = float(v_TransparencyLevel) / 255.0;
    texColor.a = mix(texColor.a, 1.0 - transparency, transparency);
    
    // Apply basic directional lighting
    vec3 lightDir = normalize(vec3(0.5, 0.8, 0.6)); // Directional light
    vec3 normal = normalize(cross(dFdx(v_WorldPos), dFdy(v_WorldPos))); // Compute normal from position
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 ambient = vec3(0.2); // Ambient light
    vec3 diffuse = diff * vec3(1.0); // Diffuse light
    vec3 lighting = ambient + diffuse;
    
    // Apply lighting to texture color
    vec3 finalColor = texColor.rgb * lighting;
    
    // Output final color
    outColor = vec4(finalColor, texColor.a);
}