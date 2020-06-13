#version 330 compatibility

layout(location = 0) in vec2 vertex;
layout(location = 1) in vec2 positions;
layout(location = 2) in vec4 colours;

out vec4 colour;

uniform mat4 transform;

void main()
{
    colour = colours;
    gl_Position = transform * vec4(vertex + positions, 0.0, 1.0);
}