#version 330 compatibility

#define PI 3.1415926

out vec4 FragColor;

uniform float timeSinceStart;
uniform sampler2D source;

void main()
{
    vec2 uv = gl_TexCoord[0].xy * 2;
    uv -= 1;
    if (length(uv) > 1.0) {
        FragColor = vec4(0.0,0.0,0.0,0.0);
        return;
    }
    uv = mix(uv,normalize(uv)*(2.0*asin(length(uv)) / PI),0.5);
    vec3 n = vec3(uv, sqrt(1.0 - uv.x*uv.x - uv.y*uv.y));
    uv = normalize(uv)*(2.0*asin(length(uv)) / PI);

    uv *= 1.3;
    uv.x += timeSinceStart * 0.03;
    uv.x *= 0.5;
    vec3 color = texture(source,uv * 0.5 + 0.5).rgb;

    FragColor = vec4(color,1.0);
}