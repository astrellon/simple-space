#version 130

out vec4 FragColor;

in vec4 colour;
uniform float alpha;

void main()
{
    FragColor = vec4(colour.rgb, colour.a * alpha);
}