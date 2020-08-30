#version 130

out vec4 FragColor;

in vec4 colour;
in float dist;

void main()
{
    float len = (clamp(30 - (dist / 10), 0, 50) / 50);
    //FragColor = mix(vec4(colour.rga, colour.a), vec4(1, 1, 1, 1), len);
    vec4 outColour = vec4(colour.rgb, colour.a * len);
    FragColor = mix(outColour, vec4(1, 1, 1, 1), len);
}
