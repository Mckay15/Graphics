#include "GameObject.h"
#include "VertexArray.h"
#include "ShaderProgram.h"
#include "Texture.h"

void GameObject::Movement(glm::vec3 _Position)
{
	model = glm::translate(model, _Position);
}

void GameObject::Reset()
{
	model = glm::mat4(1.0);
}

GameObject::GameObject(std::string _Texture, std::string _Obj)
{
	Object = new VertexArray(_Obj);
	Tex = new Texture(_Texture);
	model = glm::mat4(1.0);
	
}
GameObject::GameObject(std::string _Texture, std::string _Obj, std::string _Normal)
{
	Object = new VertexArray(_Obj);
	Tex = new Texture(_Texture);
	Normal = new Texture(_Normal);
	model = glm::mat4(1.0);
}

GameObject::~GameObject()
{
}
