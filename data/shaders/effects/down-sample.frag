#version 330 compatibility

uniform sampler2D source;
uniform vec2 sourceSize;

out vec4 FragColor;

void main()
{
    vec2 pixelSize = vec2(1.0 / sourceSize.x, 1.0 / sourceSize.y);
    vec2 textureCoordinates = gl_TexCoord[0].xy;
    vec4 color = texture2D(source, textureCoordinates);
    color     += texture2D(source, textureCoordinates + vec2( 1.0,  0.0) * pixelSize);
    color     += texture2D(source, textureCoordinates + vec2(-1.0,  0.0) * pixelSize);
    color     += texture2D(source, textureCoordinates + vec2( 0.0,  1.0) * pixelSize);
    color     += texture2D(source, textureCoordinates + vec2( 0.0, -1.0) * pixelSize);
    color     += texture2D(source, textureCoordinates + vec2( 1.0,  1.0) * pixelSize);
    color     += texture2D(source, textureCoordinates + vec2(-1.0, -1.0) * pixelSize);
    color     += texture2D(source, textureCoordinates + vec2( 1.0, -1.0) * pixelSize);
    color     += texture2D(source, textureCoordinates + vec2(-1.0,  1.0) * pixelSize);
    FragColor = color / 9.0;
}