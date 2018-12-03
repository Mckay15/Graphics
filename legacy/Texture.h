#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>

class Texture
{

private:
	GLuint id;
	glm::vec2 size;
public:
	Texture(std::string path);
	~Texture();

	GLuint getId();
	glm::vec2 getSize();
};

