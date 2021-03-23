#version 330 core

out vec4 FragColor;


in vec2 texCoord;
in vec3 fragPos;
in vec3 normal;
in vec3 lightPos;
uniform sampler2D texture1;
uniform sampler2D texture2;


void main()
{
    
    /*float ambientStrength=0.1;
    float diff=dot(normalize(fragPos-lightPos),normal);
    FragColor = mix(texture(texture1,texCoord),texture(texture2,texCoord),0.3)*(ambientStrength+diff);
    FragColor=vec4(1,0,0,1)*(ambientStrength+diff);
    //FragColor=texture(texture2,texCoord);*/
    float ambientStrength=0.1;
    float diff=dot(normalize(lightPos-fragPos),normal);
    FragColor = mix(texture(texture1,texCoord),texture(texture2,texCoord),0.3)*(ambientStrength+diff);
} 