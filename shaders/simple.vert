attribute vec3 in_Position;
attribute vec2 in_TexCoord;
attribute vec3 in_Normal;
attribute vec3 in_Tangent;
attribute vec3 in_BiTangent;

uniform mat4 in_Projection;
uniform mat4 in_Model;
uniform mat4 in_View;
uniform vec3 in_LightPos;

varying vec3 ex_FragPos;
varying vec3 ex_Normal;
varying vec2 ex_TexCoord;
varying mat3 TBN;
	  
void main ()
{
vec4 VT_View = in_View * vec4(normalize(in_Normal), 1.0);
vec4 Tan_View = in_View * vec4(normalize(in_Tangent), 1.0);
vec4 BiTan_View = in_View * vec4(normalize(in_BiTangent), 1.0);

TBN = transpose(mat3(VT_View, BiTan_View, Tan_View));

ex_Normal = mat3(in_Model) * in_Normal;
ex_FragPos = vec3(in_Model * vec4(in_Position, 1.0));

vec3 lightDir = normalize(in_LightPos - ex_FragPos);

gl_Position = in_Projection * in_Model* in_View * vec4(in_Position, 1.0);
ex_TexCoord = in_TexCoord;
} 
