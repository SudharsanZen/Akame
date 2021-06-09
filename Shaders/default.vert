#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (std140) uniform Matrices
{
    mat4 proj;
    mat4 view;
};


uniform vec3 lpose;
uniform mat4 transform;
uniform mat4 viewmat;
uniform mat4 projmat;

out vec3 Normal;
out vec3 FragPos;
out vec2 uvCoord;
void main()
{
    
    gl_Position =proj*view*transform*vec4(aPos, 1.0);
    FragPos = vec3(transform * vec4(aPos, 1.0));

    Normal=mat3(transpose(inverse(transform))) * normal;
    uvCoord=texCoord;
}