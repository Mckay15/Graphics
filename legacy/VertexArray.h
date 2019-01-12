#include <glm/glm.hpp>
#include <vector>
#include <gl/glew.h>

#include <string>

class VertexBuffer;

class VertexArray
{
private :
	int t = 0;
	bool dirty;
	std::vector <VertexBuffer*> buffers;
	GLuint id;

	glm::vec3 V1;
	glm::vec3 V2;
	glm::vec3 V3;
	glm::vec2 UV1;
	glm::vec2 UV2;
	glm::vec2 UV3;

	void SplitStringWhitespace(std::string& input, std::vector<std::string>& output);

	void SplitString(std::string& input, char splitter, std::vector<std::string>& output);
public:
	VertexArray();
	VertexArray(std::string path);

	void SetBuffer(std::string sttribute, VertexBuffer* buffer);
	int GetVertexCount();
	GLuint GetId();

	~VertexArray();
};

