#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>


class FrameBufferObject
{
private:
	unsigned int fbo;
	unsigned int texture;
	unsigned int rbo;
public:
	void ActiveBuffer();
	FrameBufferObject();
	~FrameBufferObject();
};

