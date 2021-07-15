#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec2  uvCoord;


void main()
{
    gl_Position=vec4(aPos,1);
    uvCoord=texCoord;
}