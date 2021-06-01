#version 130

uniform int pointSize;

out vec3 position;
out vec4 colour;

void main()
{
    position = gl_Vertex.xyz;
    colour = gl_Color;
    gl_PointSize = pointSize;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}