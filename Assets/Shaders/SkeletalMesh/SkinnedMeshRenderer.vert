#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;
layout (std140) uniform Matrices
{
    mat4 proj;
    mat4 view;
};



uniform mat4 transform;

uniform int t_index;
out vec3 Normal;
out vec3 FragPos;
out vec2 uvCoord;
out mat3 TBN;


void main()
{
    vec3 disp=normal;
    mat4 model=transform;
    gl_Position =proj*view*model*vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    vec3 T = normalize(vec3(model*vec4(normalize(tangent),   0.0)));
    vec3 B = normalize(vec3(model*vec4(normalize(bitangent), 0.0)));
    vec3 N = mat3(transpose(model)) * normal;
    TBN = mat3(T, B, N);
    Normal=N;
    uvCoord=texCoord;
}