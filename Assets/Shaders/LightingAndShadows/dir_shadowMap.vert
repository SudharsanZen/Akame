#version 430 core

layout (location = 0) in vec3  position;

uniform mat4 transform;
uniform mat4 cv;
//uniform mat4 cp;
uniform mat4 lightSpaceMat;


void main()
{
    //vec4 pose=projCam*viewCam*transform*vec4(position,1);
   // gl_Position=projMat*view*pose;

    vec4 pose= transform*vec4(position,1);


     gl_Position=lightSpaceMat*vec4(pose.xyz,1);
    
}