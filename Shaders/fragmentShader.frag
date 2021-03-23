#version 330 core

out vec4 FragColor;


in vec2 texCoord;
in vec3 fragPos;
in vec3 normal;
in vec3 lightPos;
uniform sampler2D texture1;
uniform vec3 viewPose;
uniform sampler2D texture2;


void main()
{
    
    /*float ambientStrength=0.1;
    float diff=dot(normalize(fragPos-lightPos),normal);
    FragColor = mix(texture(texture1,texCoord),texture(texture2,texCoord),0.3)*(ambientStrength+diff);
    FragColor=vec4(1,0,0,1)*(ambientStrength+diff);
    //FragColor=texture(texture2,texCoord);*/
    vec3 lightDir=normalize(lightPos-fragPos);
    vec3 viewDir=normalize(viewPose-fragPos);
    vec3 lightRef=reflect(-lightDir,normal);
    float spec=pow(max(dot(viewDir,lightRef),0.0),32);
    float ambientStrength=0.1;
    float diff=dot(lightDir,normal);
    FragColor = mix(texture(texture1,texCoord),texture(texture2,texCoord),0.1)*(ambientStrength+diff+(spec*1));
    //FragColor = vec4(1,0.5,0.5,1)*(ambientStrength+diff+(spec*1));
} 