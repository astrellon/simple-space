#version 130

#define PI 3.1415926

in vec2 texCoord;
out vec4 FragColor;

uniform float timeSinceStart;
uniform sampler2D source;
uniform vec2 offset;
uniform float rotationRate;

float Hash(in vec2 p, in float scale)
{
    // This is tiling part, adjusts with the scale...
    p = mod(p, scale);
    return fract(sin(dot(p, vec2(27.16898, 38.90563))) * 5151.5473453);
}

//----------------------------------------------------------------------------------------
float noise(in vec2 p, in float scale, in vec2 offset )
{
    vec2 f;

    p *= scale;


    f = fract(p);		// Separate integer from fractional
    p = floor(p);

    p += offset;

    f = f*f*(3.0-2.0*f);	// Cosine interpolation approximation

    float res = mix(mix(Hash(p,                  scale),
                        Hash(p + vec2(1.0, 0.0), scale), f.x),
                    mix(Hash(p + vec2(0.0, 1.0), scale),
                        Hash(p + vec2(1.0, 1.0), scale), f.x), f.y);
    return res;
}

//----------------------------------------------------------------------------------------
float fBm(in vec2 p)
{
    float f = 0.0;
    // Change starting scale to any integer value...
    float scale = 10.;
    p = mod(p, scale);
    float amp  = 0.6;

    for (int i = 0; i < 5; i++)
    {
        f += noise(p, scale, offset) * amp;
        amp *= .5;
        // Scale must be multiplied by an integer value...
        scale *= 2.;
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
    vec2 uv = gl_TexCoord[0].xy * 2;
    uv *= 1.2;
    uv -= 1.2;

    float len = length(uv);
    if (len > 1.0) {
        float t = max(0, 1 - ((len - 1) / 0.2));
        float p = easeOutQuad(t);
        FragColor = vec4(0.6 * p, 0.8 * p, p, p);

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