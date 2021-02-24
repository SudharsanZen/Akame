#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec2 atexCoord;

uniform float anotherColor;
uniform vec4 color;
uniform mat4 transform;

out vec3 vertexColor;
out vec2 texCoord;

void main()
{
    gl_Position = transform*vec4(aPos, 1.0);
    vertexColor = aCol;
    
    texCoord=atexCoord;
}