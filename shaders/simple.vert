attribute vec3 in_Position;
attribute vec2 in_TexCoord;
attribute vec3 in_Normal;

uniform mat4 in_Projection;
uniform mat4 in_Model;
uniform mat4 in_View;

varying vec3 ex_FragPos;
varying vec3 ex_Normal;
varying vec2 ex_TexCoord;
	  
void main()
{
ex_Normal = mat3(in_Model) * in_Normal;
ex_FragPos = vec3(in_Model * vec4(in_Position, 1.0));
gl_Position = in_Projection * in_Model* in_View * vec4(in_Position, 1.0);
ex_TexCoord = in_TexCoord;
} 
