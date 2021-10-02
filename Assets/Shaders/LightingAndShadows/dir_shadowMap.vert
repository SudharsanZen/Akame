#version 430 core

layout (location = 0) in vec3  position;



layout (std140, binding=8) uniform transformMatrices
{
    mat4 transform[1000];    
};

uniform int t_index;

void main()
{
    //vec4 pose=projCam*viewCam*transform*vec4(position,1);
   // gl_Position=projMat*view*pose;

    gl_Position=transform[t_index]*vec4(position,1);
    
}