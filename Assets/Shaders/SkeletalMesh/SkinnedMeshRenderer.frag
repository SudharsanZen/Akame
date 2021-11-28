#version 430 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
in vec2 uvCoord;
in mat3 TBN;
uniform float near=0.1f;
uniform float far=250;
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform int emissive;
uniform float normalStrength=0;
uniform sampler2DArrayShadow dir_sMap;
uniform sampler2DArray dir_Map;
uniform float shadowRes;
uniform mat4 viewMat;
uniform mat4 lightSpaceMat[5];
uniform int numOfFrustum;
uniform float lambda;
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
struct DirectionalLight
{
    vec3 lightDir;
    vec3 lightColor;
    vec3 ambient;
    float intensity;

};
uniform DirectionalLight dir;

struct BufferPixelValues
{
    vec4 albedo;
    float specular;
    vec4 norm;
    vec4 FragPos;
};
vec2 poissonDisk[16] = vec2[](
    vec2(-0.94201624, -0.39906216),
    vec2(0.94558609, -0.76890725),
    vec2(-0.094184101, -0.92938870),
    vec2(0.34495938, 0.29387760),
    vec2(-0.91588581, 0.45771432),
    vec2(-0.81544232, -0.87912464),
    vec2(-0.38277543, 0.27676845),
    vec2(0.97484398, 0.75648379),
    vec2(0.44323325, -0.97511554),
    vec2(0.53742981, -0.47373420),
    vec2(-0.26496911, -0.41893023),
    vec2(0.79197514, 0.19090188),
    vec2(-0.24188840, 0.99706507),
    vec2(-0.81409955, 0.91437590),
    vec2(0.19984126, 0.78641367),
    vec2(0.14383161, -0.14100790)
);


int layer=0;
float lightWidth=0.001f;
#define BLOCKER_SARCH_SAMPLE_COUNT 16

float divDist(float nthPlane,float lambda)
{
      //float bias = 1.0f-((numOfFrustum-nthPlane)/(numOfFrustum))-(1.0f/numOfFrustum);
	//lambda += bias;
      float logrithmic=near*pow(far/near,nthPlane/numOfFrustum);
      float uni=near+(far-near)*(nthPlane/numOfFrustum);

      return (logrithmic*(1.0f-lambda)+uni*(lambda));
}

void FindBlocker(out float avgBlockerDepth, out float numBlockers, int mapLayer, vec2 uv, float zReceiver) {
    //This uses similar triangles to compute what
    //area of the shadow map we should search
    float searchWidth = lightWidth * (zReceiver - near) / zReceiver;
    float blockerSum = 0;
    numBlockers = 0;

    for (int i = 0; i < BLOCKER_SARCH_SAMPLE_COUNT; ++i) {
        float shadowMapDepth = texture(dir_sMap, vec4(uv + poissonDisk[i] * searchWidth*(1.0f/shadowRes), mapLayer,zReceiver));
        if (shadowMapDepth < zReceiver) {
            blockerSum += shadowMapDepth;
            numBlockers++;
        }
    }
    avgBlockerDepth = blockerSum / numBlockers;
}

float PCF_Filter(int mapLayer, vec2 uv, float zReceiver, float filterRadiusUV) {
    float sum = 0.0;
    float shadow_bias = 0.1;

    for (int i = 0; i < 16; ++i) {
        vec2 offset = poissonDisk[i] * filterRadiusUV;
        float shadowMapDepth = texture(dir_sMap, vec4(uv + offset, mapLayer,zReceiver));
        sum += float(shadowMapDepth < zReceiver - shadow_bias);
    }

    return sum / 16.0f;
}
float PCSS(int mapLayer,vec3 coord)
{
      vec2 uv=coord.xy;
      float zReceiver=coord.z;
      
      float avgBlockerSize=0;
      float blockerCount=0;
      FindBlocker(avgBlockerSize,blockerCount,mapLayer,uv,zReceiver);
      if(blockerCount<1)
            return 0.0f;
      
      float zb=texture(dir_sMap, vec4(uv,layer,zReceiver));
      float penumbraSize=(zReceiver-zb)*lightWidth/zb;
      float filterRadius=penumbraSize*near/coord.z;
      filterRadius=clamp(filterRadius,0.0002,0.001);
      return PCF_Filter(mapLayer,uv,zReceiver,filterRadius);
}
float shadowCalculation(vec4 fragPose)
{
      float splitPose=0;
      
      float depth=0;
      vec4 camSpace=abs(viewMat*fragPose);
      vec3 projCoord;
      float shadow=0;
      for(int i=1;i<=numOfFrustum;i++)
      {
            splitPose=divDist(i,lambda);
            if(camSpace.z<splitPose)
            {
                  layer=i-1;
                    fragPose=lightSpaceMat[layer]*fragPose;
                  projCoord=fragPose.xyz/fragPose.w;
                  projCoord=projCoord*0.5+0.5;
                  float texel=1/shadowRes;
                  int con=2;
                  float pcf=0.0f;
                  for(int i=-con;i<=con;i++)
                  for(int j=-con;j<=con;j++)
                  {
                    pcf+=texture(dir_sMap, vec4(projCoord.xy+vec2(i,j)*texel,layer,projCoord.z));
                  }
                  pcf/=(2*con+1)*(2*con+1);
                  return 1.0f-pcf;
                  break;
            }
      }
      
  

      
      
      return 0.0f;
}

vec3 calcDirecLight(DirectionalLight l,BufferPixelValues P,float shadow)
{
    //flip light and make it's direction from fragment pose to lightSource
    vec3 lightDir=normalize(-l.lightDir);
    float diff= max(dot(P.norm.xyz, lightDir), 0.0);

    //calculate specular highlight
    vec3 viewDir=normalize(viewPos-P.FragPos.xyz);
    vec3 halfwayDir = normalize( lightDir+ viewDir);  
    float spec = pow(max(dot(P.norm.xyz, halfwayDir), 0.0), 64.0);
    vec3 specular=vec3(spec * P.specular);

    //calculate diffuse and ambient color
    vec3 diffuse=diff * l.lightColor * P.albedo.xyz;
    vec3 ambient=l.ambient*P.albedo.xyz;
    
    
    //add all the resultant values
    vec3 result=(((diffuse+specular)*l.intensity*(1.0f-shadow))+ambient);
    
    return result;
}
uniform vec3 viewDir;
void main()
{
    vec3 ptLight=vec3(viewPos);
    /*vec3 viewDir=FragPos-viewPos;
    vec3 lightDir=normalize(ptLight-FragPos);
    float len=length(viewDir);
    viewDir=normalize(viewPos);
    vec3 halfWay=normalize(lightDir-viewDir);*/
    
    float diff=max(dot(Normal,-viewDir),0);

    FragColor=vec4(vec3(0.1f)+vec3(2)*diff,1);
    //FragColor=vec4(vec3(LinearizeDepth(gl_FragCoord.z))/10,1.0);
} 