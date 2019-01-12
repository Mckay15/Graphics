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
	GameObject(std::string _Texture, std::string _Obj);
	GameObject(std::string _Texture, std::string _Obj, std::string _Normal);
	~GameObject();
};

