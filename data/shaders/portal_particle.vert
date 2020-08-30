#version 130

out vec4 colour;
out float dist;

void main()
{
    vec2 position = gl_Vertex.xy;
    colour = gl_Color;
    dist = length(position);
    if (dist > 50)
    {
        colour = vec4(0, 0, 0, 0);
    }
    else
    {
        gl_PointSize = 6 - clamp(floor(dist / 10), 1, 5);
        gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    }
}