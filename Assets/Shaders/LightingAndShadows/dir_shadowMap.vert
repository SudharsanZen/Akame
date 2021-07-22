#version 430 core

layout (location = 0) in vec3  position;

uniform mat4 transform;
uniform mat4 projMat;
uniform mat4 view;

void main()
{
    gl_Position=projMat*view*transform*vec4(position,1);
}