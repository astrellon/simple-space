#version 330 compatibility

layout(location = 0) in vec2 vertex;
layout(location = 1) in vec2 positions;
layout(location = 2) in vec4 colours;

out vec4 colour;
out float index;

uniform mat4 transform;
uniform float timeSinceStart;

void main()
{
    colour = colours;
    index = gl_InstanceID;
    gl_Position = transform * vec4(vertex + positions, 0.0, 1.0);
}