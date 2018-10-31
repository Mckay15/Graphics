#include <glm/glm.hpp>
#include <vector>
#include <gl/glew.h>

#include <vector>
#include <string>

class VertexBuffer;

class VertexArray
{
private :
	bool dirty;
	std::vector <VertexBuffer*> buffers;
	GLuint id;

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

