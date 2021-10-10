#version 430 core
layout (location = 0) out vec4 gAlbedoRougness;
layout (location = 1) out vec4 gPosition;
layout (location = 2) out vec4 gNormal;
//r:metallic g:AO
layout (location = 3) out vec4 gPBR;

in vec2 uvCoord;
in vec4 FragPos;
in vec3 Normal;
in mat3 TBN;
struct Material
{
    sampler2D diffuse;
    sampler2D roughness;
    sampler2D metallic;
    sampler2D normal;
    sampler2D ambientocclusion;
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;

};
uniform float noAO,noMetallic,noRoughness,noNormal;
uniform Material material;
uniform float metallic,roughness;
uniform float ambientocclusion;
uniform float normalStrength;

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;
    float a=texture(material.diffuse,uvCoord).a;
    if(a<0.5)
        discard;
    if(noNormal<1.0f)
    {
        vec3 nMap=texture(material.normal,uvCoord).rgb;
        nMap=normalize(nMap*2.0-1.0);
        nMap.xy*=normalStrength;
        if(normalStrength==0)
            nMap=vec3(0,0,1);
        // also store the per-fragment normals into the gbuffer
        //gNormal = normalize(Normal);
        gNormal = vec4(normalize(TBN*nMap),1);
    }
    else
        gNormal=vec4(normalize(TBN*vec3(0,0,1)),1);
    // and the diffuse per-fragment color
    gAlbedoRougness.rgb = texture(material.diffuse, uvCoord).rgb;
    // store roughness, metallic and ambient occlusion
    gAlbedoRougness.a=roughness;
    if(noRoughness<1.0f)
        gAlbedoRougness.a = texture(material.roughness, uvCoord).r;
    gPBR=vec4(metallic,ambientocclusion,0,0);
    if(noMetallic<1.0f)
        gPBR.r = texture(material.metallic, uvCoord).r;
    if(noAO<1.0f)
        gPBR.b =texture(material.ambientocclusion,uvCoord).r;
    
}  