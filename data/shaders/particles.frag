#version 330 compatibility

out vec4 FragColor;

in vec4 colour;

void main()
{
    FragColor = colour;
}