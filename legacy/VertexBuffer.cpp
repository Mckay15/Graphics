#include "VertexBuffer.h"


void VertexBuffer::add(glm::vec2 value)
{
	if (componets != 2 && componets != 0)
	{
		throw std::exception();
	}

	data.push_back(value.x);
	data.push_back(value.y);
	componets = 2;
	dirty = true;
}
void VertexBuffer::add(glm::vec3 value)
{
	if (componets != 3 && componets != 0)
	{
		throw std::exception();
	}

	data.push_back(value.x);
	data.push_back(value.y);
	data.push_back(value.z);
	componets = 3;
	dirty = true;
}
void VertexBuffer::add(glm::vec4 value)
{
	if (componets != 4 && componets != 0)
	{
		throw std::exception();
	}

	data.push_back(value.x);
	data.push_back(value.y);
	data.push_back(value.z);
	data.push_back(value.w);
	componets = 4;
	dirty = true;
}

int VertexBuffer::GetComponet()
{
	return componets;
}
int VertexBuffer::GetDataSize()
{
	return data.size();
}
GLuint VertexBuffer::GetId()
{
	if (dirty)
	{
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data.at(0), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		dirty = false;
	}
	return id;
}

VertexBuffer::VertexBuffer()
{
	componets = 0;
	dirty = false;

	glGenBuffers(1, &id);

	if (!id)
	{
		throw std::exception();
	}

}


VertexBuffer::~VertexBuffer()
{
}
