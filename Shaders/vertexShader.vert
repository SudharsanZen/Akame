#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 atexCoord;


uniform mat4 transform;
uniform mat4 proj;
uniform mat4 view;

out vec3 vertexColor;
out vec2 texCoord;

void main()
{
    gl_Position =transform*vec4(aPos, 1.0);
    gl_Position =view*gl_Position;
    gl_Position =proj*gl_Position;
    
    texCoord=atexCoord;
}