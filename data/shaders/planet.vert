#version 130

in vec4 vertex;

void main()
{
    gl_Position = vertex;
    gl_TexCoord[0] = gl_MultiTexCoord0;
}