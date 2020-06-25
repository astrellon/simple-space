#version 130

uniform float distanceScale;
uniform float zoomScale;

out vec2 position;
out vec4 colour;

void main()
{
    position = gl_Vertex.xy;
    colour = gl_Color;
    gl_PointSize = distanceScale * zoomScale;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}