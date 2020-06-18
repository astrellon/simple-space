#version 330 compatibility

layout(location = 0) in vec3 vertex;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(vertex, 1);
}
