#include <glm/glm.hpp>
#include <vector>
#include <gl/glew.h>

class VertexBuffer
{
private:
	GLuint id;
	int componets;
	std::vector <GLfloat> data;
	bool dirty;
public:
	void add(glm::vec2 value);
	void add(glm::vec3 value);
	void add(glm::vec4 value);

	int GetComponet();
	int GetDataSize();
	GLuint GetId();

	VertexBuffer();
	~VertexBuffer();
};

