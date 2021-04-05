#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in mat4 transform;

uniform mat4 proj;
uniform mat4 view;
uniform vec3 lpose;



out vec3 Normal;
out vec3 FragPos;
out vec3 lightPos;
out vec2 uvCoord;
void main()
{
    
    gl_Position =proj*view*transform*vec4(aPos, 1.0);
    FragPos = vec3(transform * vec4(aPos, 1.0));

    Normal=mat3(transpose(inverse(transform))) * normal;
    lightPos=lpose;
    uvCoord=texCoord;
}