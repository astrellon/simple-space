#version 330 compatibility

uniform mat4 transform;

void main()
{
	gl_Position = gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
}