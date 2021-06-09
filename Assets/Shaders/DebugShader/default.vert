#version 330 core

layout (location = 0) in vec3 aPos;

layout (std140) uniform Matrices
{
    mat4 proj;
    mat4 view;
};

void main()
{
    
    gl_Position =proj*view*vec4(aPos, 1.0);

}