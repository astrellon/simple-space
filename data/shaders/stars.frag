#version 130

#define E 2.71828

out vec4 FragColor;

in vec3 position;
in vec4 colour;

uniform float timeSinceStart;

const vec4 twinkleColour = vec4(0.3, 0.3, 0.3, 0.2);

void main()
{
    float twinkleFactor = mod(timeSinceStart + position.z, 5) - 2.5;
    float twinkleMultiplier = pow(E, -(twinkleFactor * twinkleFactor / 0.1));
    float darken = ((500 + position.z) / 500) * 0.5 + 0.5;
    FragColor = colour * clamp(darken, 0, 1) + (twinkleColour * twinkleMultiplier * 1.3);
}
