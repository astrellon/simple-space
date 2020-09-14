#version 130

#define MAX_BLADE_LENGTH 10.0f
#define PI 3.1415926

uniform sampler2D source;
out vec4 FragColor;

uniform vec2 invTextureSize;
uniform vec4 tipColour;
uniform vec4 sideColour;

void main()
{
    vec2 uv = gl_TexCoord[0].xy * invTextureSize;
    //FragColor = texture2D(source, vec2(uv.x / textureSize.x, uv.y / textureSize.y));
    // FragColor = vec4(uv, 0, 1);
    FragColor = vec4(0.0f); // Start with clear color.
    for (float dist = 0.0f; dist < MAX_BLADE_LENGTH; ++dist)
    {
        float blade_length = texture2D(source, uv).r * 255.0f;

        if (blade_length > 0.0f)
        {
            if (dist == blade_length)
            {
                FragColor = tipColour;
            }
            else if (dist < blade_length)
            {
                FragColor = sideColour;
            }
        }

        uv += vec2(0.0f, invTextureSize.y);
    }
}