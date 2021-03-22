#version 330 core

out vec4 FragColor;

in vec3 vertexColor;
in vec2 texCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;


void main()
{
    float ambientStrength=0.1;
    FragColor = mix(texture(texture1,texCoord),texture(texture2,texCoord),0.3)*ambientStrength;
    //FragColor=texture(texture2,texCoord);
} 