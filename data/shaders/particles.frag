#version 330 compatibility

out vec4 FragColor;

in vec4 colour;
in float index;

uniform float timeSinceStart;

void main()
{
    FragColor = colour + vec4(0.1, 0.1, 0.1, 0) * sin(index + timeSinceStart * 10) * cos(timeSinceStart * 4 + index * 3);
}