#version 130

#define PI 3.1415926

in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D source;
uniform float timeSinceStart;
uniform float rotationRate;
uniform float offset;
uniform float scale;
uniform vec4 glowColour;

float Hash(in vec2 p, in float localScale)
{
    // This is tiling part, adjusts with the scale...
    p = mod(p, localScale);
    return fract(sin(dot(p, vec2(27.16898, 38.90563))) * 5151.5473453);
}

//----------------------------------------------------------------------------------------
float noise(in vec2 p, in float localScale)
{
    vec2 f;

    p *= localScale;


    f = fract(p);		// Separate integer from fractional
    p = floor(p);

    p += offset;

    f = f*f*(3.0-2.0*f);	// Cosine interpolation approximation

    float res = mix(mix(Hash(p,                  localScale),
                        Hash(p + vec2(1.0, 0.0), localScale), f.x),
                    mix(Hash(p + vec2(0.0, 1.0), localScale),
                        Hash(p + vec2(1.0, 1.0), localScale), f.x), f.y);
    return res;
}

//----------------------------------------------------------------------------------------
float fBm(in vec2 p)
{
    float f = 0.0;
    // Change starting scale to any integer value...
    float localScale = scale;
    p = mod(p, localScale);
    float amp  = 0.6;

    for (int i = 0; i < 5; i++)
    {
        f += noise(p, localScale) * amp;
        amp *= .5;
        // Scale must be multiplied by an integer value...
        localScale *= 2.;
    }
    // Clamp it just in case....
    return min(f, 1.0);
}

float easeOutQuad(float t)
{
    return t * t;
}

void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    uv *= 1.2;
    uv -= 1.2;

    float len = length(uv);
    if (len > 1.0) {
        float t = max(0, 1 - ((len - 1) / 0.2));
        float p = easeOutQuad(t);
        FragColor = glowColour * p;

        return;
    }
    uv = mix(uv,normalize(uv)*(2.0*asin(len) / PI),0.5);
    vec3 n = vec3(uv, sqrt(1.0 - uv.x*uv.x - uv.y*uv.y));
    uv = normalize(uv)*(2.0*asin(length(uv)) / PI);

    uv *= 1.3;
    uv.y *= 0.5;
    uv.x += timeSinceStart * rotationRate;
    uv.x *= 0.25;

    float noise = fBm(uv);
    vec4 colour = texture2D(source, vec2(noise, 0));

    FragColor = colour;
}