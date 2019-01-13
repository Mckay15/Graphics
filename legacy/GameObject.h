#include <glm/glm.hpp>
#include <gl/glew.h>
#include <glm/ext.hpp>
#include <glm/common.hpp>
#include <string>

class VertexArray;
class ShaderProgram;
class Texture;

class GameObject
{
private:
	ShaderProgram* Shader;
	VertexArray* Object;
	Texture* Tex;
	Texture* Normal;
	glm::mat4 model;
public:
	Texture* GetTexture() { return Tex; }
	VertexArray* GetVAO() { return Object; }
	Texture* GetNormalMap() { return Normal; }
	void Movement(glm::vec3 _Position);
	void Reset();
	glm::mat4 GetModel() { return model; };
	int tr_tri_intersect3D(double *C1, double *P1, double *P2,
		double *D1, double *Q1, double *Q2);
	GameObject(std::string _Texture, std::string _Obj);
	GameObject(std::string _Texture, std::string _Obj, std::string _Normal);
	~GameObject();
};

