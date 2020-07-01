#version 130

out vec4 FragColor;

uniform float alpha;

void main()
{
    FragColor = vec4(0.1, 0.15, 0.2, alpha);
}