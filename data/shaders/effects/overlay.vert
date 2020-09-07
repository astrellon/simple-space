#version 130

out vec4 colour;

void main()
{
    colour = gl_Color;
    gl_Position = gl_Vertex;
}