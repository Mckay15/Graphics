uniform sampler2D in_Texture;
uniform vec3 in_Emissive;
uniform vec3 in_Ambient;
uniform vec3 in_LightPos;
uniform sampler2D in_NormalMap;

uniform mat4 in_View;

varying vec2 ex_TexCoord;
varying vec3 ex_FragPos;
varying vec3 ex_Normal;
varying mat3 TBN;

void main()
{
	vec3 Norm = normalize(ex_Normal);
	vec3 lightDir = normalize(in_LightPos - ex_FragPos);

	vec4 tex = texture2D(in_Texture, ex_TexCoord);

	vec4 texNorm = texture2D(in_NormalMap, ex_TexCoord);

	vec3 TexNorm_Tan = normalize(texture2D(in_NormalMap, ex_TexCoord).rgb * 2 - 1);

	float diff = clamp(dot(TexNorm_Tan, lightDir), 0,1);
	vec3 Diffuse = diff * vec3(1,1,1);

	vec3 viewPos = vec3(inverse(in_View) * vec4(0,0,0,1));
	vec3 viewDir = normalize(viewPos - ex_FragPos);
	vec3 reflectDir = reflect(-lightDir, Norm);
	float Spec = pow(max(dot((viewDir), reflectDir), 0.0),16);
	vec3 Specular = Spec * vec3(1,1,1);

	vec3 lighting =  in_Emissive + in_Ambient + Diffuse + Specular;

	gl_FragColor = tex * vec4(lighting, 1);
}