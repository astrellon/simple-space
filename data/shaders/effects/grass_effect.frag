#version 130

#define MAX_BLADE_LENGTH 10.0f
#define PI 3.1415926

uniform sampler2D source;
uniform sampler2D noiseTex;
out vec4 FragColor;

uniform vec2 invTextureSize;
uniform vec2 invNoiseTexSize;
uniform vec4 tipColour;
uniform vec4 sideColour;
uniform vec4 windColour;
uniform float windSpeed;
uniform vec2 windDirection;
uniform float timeSinceStart;

float sineWave(float T, float a, float phase, vec2 dir, vec2 pos)
{
    return a * sin(2.0f * PI / T * dot(dir, pos) + phase);
}

float wind (vec2 pos, float t) {
    return (sineWave(1.0f, 1.8f, 1.0f*windSpeed*t,
                   windDirection, pos)
          + sineWave(0.35f, 0.1f, 2.0f*windSpeed*t,
                     normalize(windDirection - vec2(0.0f, 0.4f)), pos)
          + sineWave(0.375f, 0.1f, 1.5f*windSpeed*t,
                     normalize(windDirection + vec2(0.4f, 0.0f)), pos))
         / 3.0f;
}

float sampleNoise(vec2 uv, float offset)
{
    return texture2D(noiseTex, vec2(uv.x * invNoiseTexSize.x + offset + uv.y * 13.0f, 0.0f)).r;
}

void main()
{
    vec2 uv = gl_TexCoord[0].xy * invTextureSize;
    float noise = sampleNoise(gl_TexCoord[0].xy, 0.1f * windSpeed * timeSinceStart);

    uv.y -= noise * invNoiseTexSize.x * 0.25f;

    FragColor = vec4(0.0f); // Start with clear color.
    for (float dist = 0.0f; dist < MAX_BLADE_LENGTH; ++dist)
    {
        float wind = wind(uv, -timeSinceStart);
        float blade_length = texture2D(source, uv).r * 255.0f;

        if (blade_length > 0.0f)
        {
            if (wind > 0.5f)
            {
                blade_length -= 1.0f;
            }

            if (dist == blade_length)
            {
                if (wind <= 0.5f)
                {
                    FragColor = tipColour;
                }
                else
                {
                    FragColor = windColour;
                }
            }
            else if (dist < blade_length)
            {
                FragColor = sideColour;
            }
        }

        uv += vec2(0.0f, invTextureSize.y);
    }
}