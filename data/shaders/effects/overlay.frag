#version 130

out vec4 FragColor;

uniform float alpha;

void main()
{
    FragColor = vec4(0, 0, 0, alpha);
}