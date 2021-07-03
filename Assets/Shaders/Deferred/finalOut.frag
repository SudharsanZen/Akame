#version 330 core

out vec4 FragColor;
  
in vec2 uvCoord;



uniform sampler2D Position;
uniform sampler2D Normal;
uniform sampler2D AlbedoSpec;


uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform int emissive;

const int NR_LIGHTS = 32;



void main()
{            
    
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	

    vec3 norm = normalize(texture(Normal,uvCoord).xyz);
    vec3 FragPos=texture(Position,uvCoord).xyz;
    //norm = normalize(texture(material.normalMap,uvCoord).xyz);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = 1*pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = texture(AlbedoSpec,uvCoord).a * spec * lightColor;  
        
    vec3 result = (ambient + diffuse + specular) * texture(AlbedoSpec,uvCoord).xyz;
    //vec3 result = (ambient + diffuse + specular)*(objectColor);
    FragColor = vec4(result, 1.0);
}  