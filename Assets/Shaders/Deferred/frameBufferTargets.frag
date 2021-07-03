#version 330 core
layout (location = 0) out vec4 gAlbedoSpec;
layout (location = 1) out vec3 gPosition;
layout (location = 2) out vec3 gNormal;


in vec2 uvCoord;
in vec3 FragPos;
in vec3 Normal;

struct Material
{
    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D normalMap;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;

};
uniform Material material;
void main()
{    
    // store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;
    
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(Normal);
    // and the diffuse per-fragment color
    gAlbedoSpec.rgb = texture(material.diffuseMap, uvCoord).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = texture(material.specularMap, uvCoord).r;
}  