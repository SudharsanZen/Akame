#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 atexCoord;


uniform mat4 transform;
uniform mat4 proj;
uniform mat4 view;
uniform vec3 lpose;


out vec2 texCoord;
out vec3 normal;
out vec3 fragPos;
out vec3 lightPos;
void main()
{
    
    gl_Position =proj*view*transform*vec4(aPos, 1.0);
    fragPos = vec3(transform * vec4(aPos, 1.0));

    normal=mat3(transpose(inverse(transform))) * Normal;
    lightPos=lpose;
    texCoord=atexCoord;
}