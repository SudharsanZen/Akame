#version 330 core


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 Normal;





layout (std140) uniform Matrices
{
    mat4 proj;
    mat4 view;
};


out vec3 fragPos;
out vec3 nearPoint;
out vec3 farPoint;

vec3 unProject(float x,float y,float z)
{
    mat4 invProj=inverse(proj);  
    mat4 invView=inverse(view);

    vec4 point= (invView*invProj*vec4(x,y,z,1));

    return point.xyz/point.w;
}
void main()
{
    fragPos = aPos;

    gl_Position=vec4(fragPos,1);
    
    nearPoint=unProject(aPos.x,aPos.y,0);
    farPoint=unProject(aPos.x,aPos.y,1);
    

}