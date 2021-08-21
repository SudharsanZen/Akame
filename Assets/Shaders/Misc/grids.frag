#version 330 core

out vec4 FragColor;




in vec3 nearPoint;
in vec3 farPoint;

uniform vec3 viewPose;

layout (std140) uniform Matrices
{
    mat4 proj;
    mat4 view;
};
float getParamT(float nearP,float farP)
{
    return -nearP/(farP-nearP);
}



#define axisLength 0.01
vec4 grid(vec3 fragPose, float scale) {
  
    vec4 color=vec4(0);
    vec2 coord=fragPose.xz/3;
    
    //xy grids
    vec2 grid=abs(fract(coord-0.5)-0.5)/fwidth(coord*2);
    float line = min(grid.x, grid.y);
    vec4 gridCOl = vec4((1.0 - min(line, 1.0))*0.8f);
    
    
    vec2 gridAx=fwidth(coord);
    //x axis
    float linex=smoothstep(axisLength-gridAx.x,axisLength+gridAx.x,abs(coord.x));
    vec4 colx = vec4(1.0 - min(linex, 1.0),0,0,1);
    //z axis
    float linez=smoothstep(axisLength-gridAx.y,axisLength+gridAx.y,abs(coord.y));
    vec4 colz = vec4(0,0,1.0 - min(linez, 1.0),1);


    color=mix(gridCOl,colx,1-linex);
    color=mix(color,colz,1-linez);

    return color;
}
float getDepth(vec3 pos) {
    vec4 NDC = proj * view * vec4(pos.xyz, 1.0);
    return (NDC.z / NDC.w)*0.5f+0.5f;
}


void main()
{
    
   


    float t=getParamT(nearPoint.y,farPoint.y);

  
 
    vec3 worldPose=nearPoint*t/(farPoint-nearPoint);
    vec3 planePoint = nearPoint + t * (farPoint - nearPoint);
    
    
    FragColor = grid(planePoint, 100) * float(t > 0);
 

    gl_FragDepth=getDepth(planePoint);

} 



