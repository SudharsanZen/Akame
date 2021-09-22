#version 430 core
layout (location = 0) out vec4 gAlbedoSpec;
layout (location = 1) out vec4 gPosition;
layout (location = 2) out vec4 gNormal;


in vec2 uvCoord;
in vec4 FragPos;
in vec3 Normal;
in mat3 TBN;
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
uniform float specIntensity;
uniform float normalStrength;
void main()
{    
    // store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;
    float a=texture(material.diffuseMap,uvCoord).a;
    if(a<0.5)
        discard;
    vec3 nMap=texture(material.normalMap,uvCoord).rgb;
    nMap=normalize(nMap*2.0-1.0);
    nMap.xy*=normalStrength;
    nMap=normalize(nMap);
    // also store the per-fragment normals into the gbuffer
    //gNormal = normalize(Normal);
    gNormal = vec4(normalize(TBN*nMap),1);
    // and the diffuse per-fragment color
    gAlbedoSpec.rgb = texture(material.diffuseMap, uvCoord).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = texture(material.specularMap, uvCoord).r * specIntensity;
}  