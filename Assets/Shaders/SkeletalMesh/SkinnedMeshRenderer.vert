#version 430 core
#define MAX_BONES 100
#define MAX_BONES_PER_VERT 4
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;
layout (location = 5) in vec4 boneWeights;
layout (location = 6) in ivec4 boneIds;

layout (std140) uniform Matrices
{
    mat4 proj;
    mat4 view;
};



uniform mat4 transform;

uniform int t_index;
out vec3 Normal;
out vec3 FragPos;
out vec2 uvCoord;
out mat3 TBN;

uniform mat4 boneTransform[MAX_BONES];
void main()
{
    vec3 disp=normal;
    mat4 model=transform;
    vec4 pose=vec4(0);
    //assumes every vert has at least one bone that influences it
    for(int i=0;i<MAX_BONES_PER_VERT;i++)
    {
        if(boneIds[i]==-1)
            continue;
        if(boneIds[i]>=MAX_BONES)
        {
            pose=model*vec4(aPos,1);
            break;
        }
        vec4 locPose=(boneTransform[boneIds[i]]*vec4(aPos,1));
        pose+=boneWeights[i]*locPose;
    }
    //FragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = proj*view*pose;
    vec3 T = normalize(vec3(model*vec4(normalize(tangent),   0.0)));
    vec3 B = normalize(vec3(model*vec4(normalize(bitangent), 0.0)));
    vec3 N = mat3(transpose(model)) * normal;
    TBN = mat3(T, B, N);
    Normal=N;
    uvCoord=texCoord;
}