out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
    vec3 col = texture2D(screenTexture, TexCoords).rgb;
    FragColor = vec4(col, 1.0);
} 