#version 330 core

layout (location = 0 ) in vec3 pose;
layout (location = 1 ) in vec3 vertCol;
layout (std140) uniform Matrices
{
    mat4 proj;
    mat4 view;
};
out vec3 color;
void main()
{
    color=vertCol;
    gl_Position=proj*view*vec4(pose,1);
}