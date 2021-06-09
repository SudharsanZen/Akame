#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
in vec2 uvCoord;

uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform int emissive;

struct Material
{
    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D dispMap;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;

};
float near = 0.01; 
float far  = 10.0;
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}
uniform Material material;


void main()
{
    // ambient
    if(emissive>0)
    {
        FragColor=texture(material.diffuseMap,uvCoord);
        return;
    }
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    //norm = normalize(texture(material.normalMap,uvCoord).xyz);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    vec3 specularStrength = texture(material.specularMap,uvCoord).xyz;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = 1*pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = specularStrength.x * spec * lightColor;  
        
    vec3 result = (ambient + diffuse + specular) * texture(material.diffuseMap,uvCoord).xyz;
    //vec3 result = (ambient + diffuse + specular)*(objectColor);
    FragColor = vec4(result, 1.0);
    //FragColor=vec4(vec3(LinearizeDepth(gl_FragCoord.z))/10,1.0);
} 