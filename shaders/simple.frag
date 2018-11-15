uniform sampler2D in_Texture;
uniform vec3 in_Emissive;
uniform vec3 in_Ambient;
varying vec2 ex_TexCoord;

void main()
{
vec4 tex = texture2D(in_Texture, ex_TexCoord);
vec3 lighting = in_Emissive + in_Ambient;
gl_FragColor = tex * vec4(lighting,1);
}