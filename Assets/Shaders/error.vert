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



void main()
{
  
    gl_Position =proj*view*transform*vec4(aPos, 1.0);



}