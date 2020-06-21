#version 140

#define E 2.71828

out vec4 FragColor;

in vec4 colour;
in float index;

uniform float timeSinceStart;
uniform float distanceScale;

void main()
{
    float x = mod(timeSinceStart + index, 5) - 2.5;
    FragColor = colour + (vec4(0.3, 0.3, 0.3, 0.2) * pow(E, -(x * x / 0.1)) * (distanceScale + 0.3));
}
