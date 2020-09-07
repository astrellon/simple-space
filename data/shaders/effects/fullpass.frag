#version 130

uniform sampler2D source;

out vec4 FragColor;

void main()
{
    FragColor = texture2D(source, gl_TexCoord[0].xy);
}