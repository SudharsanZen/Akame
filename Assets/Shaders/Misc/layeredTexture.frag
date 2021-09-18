#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2DArray screenTexture;
uniform int layer;
void main()
{ 
    FragColor =texture(screenTexture,vec3(TexCoords,layer));
}