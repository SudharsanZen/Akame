#version 430 core
layout (triangles,invocations=3) in;
layout (triangle_strip, max_vertices = 3) out;
 



uniform int numOfFrustum;
uniform mat4 lightSpaceMat[5];

void main() {    
    for(int i=0;i<gl_in.length();++i)
    {
        gl_Layer=gl_InvocationID;
        gl_Position=lightSpaceMat[gl_InvocationID]*gl_in[i].gl_Position;
        EmitVertex();       
    }    
    EndPrimitive();
}  