uniform sampler2D in_Texture;
uniform vec3 in_Emissive;
uniform vec3 in_Ambient;
uniform vec3 in_LightPos;

varying vec2 ex_TexCoord;
varying vec3 ex_FragPos;
varying vec3 ex_Normal;

void main()
{
vec3 Norm = normalize(ex_Normal);
vec3 lightDir = normalize(in_LightPos - ex_FragPos);

vec4 tex = texture2D(in_Texture, ex_TexCoord);

float diff = max(dot(Norm, lightDir), 0.0);
vec3 diffuse = diff * vec3(tex);

vec3 lighting = in_Emissive + in_Ambient + diffuse;

gl_FragColor = tex * vec4(lighting, 1);
}