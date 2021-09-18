#version 430 core

layout (location = 0) in vec3  position;

uniform mat4 transform;



void main()
{
    //vec4 pose=projCam*viewCam*transform*vec4(position,1);
   // gl_Position=projMat*view*pose;

    gl_Position=transform*vec4(position,1);
    
}