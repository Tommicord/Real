#version 450

// Input from vertex shader
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

// Input from vertex shader
layout (location = 0) in vec3 v_WorldPos[];
layout (location = 1) in vec2 v_TexCoords[];
layout (location = 2) in uint v_LightingEmit[];
layout (location = 3) in uint v_TransparencyLevel[];
layout (location = 4) in uint v_FaceIndex[];

// Output to fragment shader
layout (location = 0) out vec3 g_WorldPos;
layout (location = 1) out vec2 g_TexCoords;
layout (location = 2) out uint g_LightingEmit;
layout (location = 3) out uint g_TransparencyLevel;
layout (location = 4) out uint g_FaceIndex;

// Projection-View-Model matrix
layout(push_constant) uniform PushConstants {
    mat4 model;
    mat4 view;
    mat4 projection;
} pc;

const uint maxFaces = 6;

// Frustum planes (left, right, top, bottom, near, far)
layout (binding = 3) uniform vec4 u_FrustumPlanes[6];

// Check if a point is inside the frustum
bool IsInFrustum(vec3 point) {
    for (int i = 0; i < maxFaces; ++i) {
        vec4 plane = u_FrustumPlanes[i];
        float distance = dot(plane.xyz, point) + plane.w;
        if (distance < 0.0) {
            return false;
        }
    }
    return true;
}

// Check if triangle is potentially visible
bool IsTriangleVisible(vec3 p0, vec3 p1, vec3 p2) {
    // If any vertex is inside frustum, triangle is potentially visible
    if (IsInFrustum(p0) || IsInFrustum(p1) || IsInFrustum(p2)) {
        return true;
    }
    return false;
}

void main() {
    // Get triangle vertices in world space
    vec3 p0 = v_WorldPos[0];
    vec3 p1 = v_WorldPos[1];
    vec3 p2 = v_WorldPos[2];
    
    // Perform frustum culling
    if (IsTriangleVisible(p0, p1, p2)) {
        // Emit triangle vertices
        for (int i = 0; i < 3; i++) {
            // Pass world position
            g_WorldPos = v_WorldPos[i];
            // Pass texture coordinates
            g_TexCoords = v_TexCoords[i];
            // Pass lighting and transparency
            g_LightingEmit = v_LightingEmit[i];
            g_TransparencyLevel = v_TransparencyLevel[i];
            // Pass face index
            g_FaceIndex = v_FaceIndex[i];
            // Emit vertex
            gl_Position = gl_in[i].gl_Position;
            EmitVertex();
        }
        EndPrimitive();
    }
}