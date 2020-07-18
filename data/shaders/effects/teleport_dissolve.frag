#version 130

out vec4 FragColor;

uniform sampler2D source;
uniform float amount;

void main()
{
    vec4 colour = texture2D(source, gl_TexCoord[0].xy);

    FragColor = vec4(colour.rgb, colour.a * amount);
}