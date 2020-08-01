#version 130

#define PI 3.1415926

out vec4 FragColor;

uniform sampler2D source;
uniform float amount;
uniform float offset;
uniform float aspectRatio;

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
    float localScale = 10;
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

void main()
{
    vec4 colour = texture2D(source, gl_TexCoord[0].xy);

    vec2 uv = gl_TexCoord[0].xy;
    uv -= 0.5;
    uv.x *= aspectRatio;
    uv /= aspectRatio;

    float len = length(uv);
    float dist = (len + amount - 0.5) * 3;
    float power = -0.5f * (5 * (dist - 1)) * (5 * (dist - 1));
    float a = pow(4, power);
    float cutoff = (amount + 0.15) > (1 - len) ? 1 : 0;
    float a2 = pow(a, 10);

    uv += 1e5;
    uv.x += offset;
    float noise = fBm(uv * 3) * 0.75 + 0.5;
    a *= noise;
    a2 *= noise;

    FragColor = vec4(colour.rgb, cutoff) + (vec4(0.2, 0.4, 0.9 , 1) * a) + (vec4(1, 1, 1, 1) * a2);
}