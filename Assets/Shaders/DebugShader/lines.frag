#version 330 core

in vec3 color;

out vec4 fragOut;
void main()
{
    fragOut=vec4(color,1);
}