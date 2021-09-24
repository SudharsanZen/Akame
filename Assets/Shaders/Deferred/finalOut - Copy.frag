#version 430 core

out vec4 FragColor;
  
in vec2 uvCoord;

uniform int NR_POINT_L;
uniform int NR_DIR_L;

struct DirectionalLight
{
    vec3 lightDir;
    vec3 lightColor;
    vec3 ambient;
    float intensity;

};

struct PointLight
{
    vec3 lightPose;
    vec3 lightColor;
    vec3 ambient;

    float intensity;

    vec3 constants;

};

struct BufferPixelValues
{
    vec3 albedo;
    float specular;
    vec3 norm;
    vec3 FragPos;
};
uniform sampler2D Position;
uniform sampler2D Normal;
uniform sampler2D AlbedoSpec;



uniform vec3 viewPos; 
uniform int emissive;

uniform DirectionalLight DIR_L[4];
uniform PointLight POINT_L[150];

vec3 calcDirecLight(DirectionalLight l,BufferPixelValues P)
{
    //flip light and make it's direction from fragment pose to lightSource
    float diff= max(dot(P.norm, -l.lightDir), 0.0);

    //calculate specular highlight
    vec3 reflectDir = reflect(l.lightDir, P.norm);  
    vec3 viewDir=normalize(viewPos-P.FragPos);
    float spec=pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular=vec3(spec * P.specular);

    //calculate diffuse and ambient color
    vec3 diffuse=diff * l.lightColor * P.albedo;
    vec3 ambient=l.ambient*P.albedo;
    
    
    //add all the resultant values
    vec3 result=(((diffuse+specular)*l.intensity)+ambient);
    
    return result;
}


vec3 calcPointLight(PointLight ptl,BufferPixelValues P)
{
    vec3 lightDir= ptl.lightPose-P.FragPos;;
    float lightDistance=length(lightDir);
    lightDir=normalize(lightDir);
    vec3 relectfLight=reflect(-lightDir,P.norm);
    vec3 viewDir=-normalize(P.FragPos-viewPos);
    float attenuation=1.0/( ptl.constants.x + ptl.constants.y*lightDistance + pow(lightDistance,2)*ptl.constants.z);

    float diff=max(dot(P.norm,lightDir),0);

    float spec=pow(max(dot(relectfLight,viewDir),0.0),128);

    vec3 diffuse=diff*(P.albedo*ptl.lightColor);
    vec3 ambient=ptl.ambient*P.albedo;
    vec3 specular= vec3(spec*P.specular);
    vec3 result=(attenuation)*((diffuse+specular)*ptl.intensity+ambient);
    return result;

}
BufferPixelValues pV;
void main()
{            
   
    pV.norm = normalize(texture(Normal,uvCoord).xyz);
    pV.FragPos=texture(Position,uvCoord).xyz;
    pV.specular = texture(AlbedoSpec,uvCoord).a;  
    pV.albedo=texture(AlbedoSpec,uvCoord).xyz;
    
    vec3 result=vec3(0,0,0);
    
    for(int i=0;i<NR_DIR_L;i++)
    {
        result+=calcDirecLight(DIR_L[i],pV);
    }

    for(int i=0;i<NR_POINT_L;i++)
    {
        result+=calcPointLight(POINT_L[i],pV);
    }
    
    FragColor = vec4(result, 1.0);
}  