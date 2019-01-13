#include "VertexArray.h"
#include "VertexBuffer.h"

#include <fstream>
#include <iostream>

void VertexArray::SplitStringWhitespace(std::string& input, std::vector<std::string>& output)
{
	std::string curr;

	output.clear();

	for (size_t i = 0; i < input.length(); i++)
	{
		if (input.at(i) == ' ' ||
			input.at(i) == '\r' ||
			input.at(i) == '\n' ||
			input.at(i) == '\t')
		{
			if (curr.length() > 0)
			{
				output.push_back(curr);
				curr = "";
			}
		}
		else
		{
			curr += input.at(i);
		}
	}

	if (curr.length() > 0)
	{
		output.push_back(curr);
	}
}


void VertexArray::SplitString(std::string& input, char splitter, std::vector<std::string>& output)
{
	std::string curr;

	output.clear();

	for (size_t i = 0; i < input.length(); i++)
	{
		if (input.at(i) == splitter)
		{
			output.push_back(curr);
			curr = "";
		}
		else
		{
			curr += input.at(i);
		}
	}

	if (curr.length() > 0)
	{
		output.push_back(curr);
	}
}

VertexArray::VertexArray(std::string path) : dirty(false)
{
	glGenVertexArrays(1, &id);

	if (!id)
	{
		throw std::exception();
	}

	buffers.resize(10);
	std::ifstream file(path.c_str());

	if (!file.is_open())
	{
		throw std::exception();
	}

	std::string line;
	std::vector<std::string> splitLine;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangent;
	std::vector<glm::vec3> BiTangent;

	VertexBuffer *positionBuffer = NULL;
	VertexBuffer *texCoordBuffer = NULL;
	VertexBuffer *normalBuffer = NULL;
	VertexBuffer *tangentBuffer = NULL;
	VertexBuffer *BiTangentBuffer = NULL;


	while (!file.eof())
	{
		std::getline(file, line);
		if (line.length() < 1) continue;
		SplitStringWhitespace(line, splitLine);
		if (splitLine.size() < 1) continue;

		if (splitLine.at(0) == "v")
		{
			if (!positionBuffer) positionBuffer = new VertexBuffer();

			positions.push_back(glm::vec3(
				atof(splitLine.at(1).c_str()),
				atof(splitLine.at(2).c_str()),
				atof(splitLine.at(3).c_str())));
			/*V1 = atof(splitLine.at(1).c_str());
			V2 = atof(splitLine.at(2).c_str());
			V3 = atof(splitLine.at(3).c_str());*/
		}
		else if (splitLine.at(0) == "vt")
		{
			if (!texCoordBuffer) texCoordBuffer = new VertexBuffer();

			texCoords.push_back(glm::vec2(
				atof(splitLine.at(1).c_str()),
				1.0f - atof(splitLine.at(2).c_str())));
			/*U = atof(splitLine.at(1).c_str());
			V = 1.0f - atof(splitLine.at(2).c_str());*/
		}
		else if (splitLine.at(0) == "vn")
		{
			if (!normalBuffer) normalBuffer = new VertexBuffer();

			normals.push_back(glm::vec3(
				atof(splitLine.at(1).c_str()),
				atof(splitLine.at(2).c_str()),
				atof(splitLine.at(3).c_str())));
		}
		else if (splitLine.at(0) == "f")
		{
			std::vector<std::string> subsplit;
			SplitString(splitLine.at(1), '/', subsplit);

			positionBuffer->add(positions.at(atoi(subsplit.at(0).c_str()) - 1));
			if (texCoordBuffer) texCoordBuffer->add(texCoords.at(atoi(subsplit.at(1).c_str()) - 1));
			if (normalBuffer) normalBuffer->add(normals.at(atoi(subsplit.at(2).c_str()) - 1));
			SplitString(splitLine.at(2), '/', subsplit);
			positionBuffer->add(positions.at(atoi(subsplit.at(0).c_str()) - 1));
			if (texCoordBuffer) texCoordBuffer->add(texCoords.at(atoi(subsplit.at(1).c_str()) - 1));
			if (normalBuffer) normalBuffer->add(normals.at(atoi(subsplit.at(2).c_str()) - 1));
			SplitString(splitLine.at(3), '/', subsplit);
			positionBuffer->add(positions.at(atoi(subsplit.at(0).c_str()) - 1));
			if (texCoordBuffer) texCoordBuffer->add(texCoords.at(atoi(subsplit.at(1).c_str()) - 1));
			if (normalBuffer) normalBuffer->add(normals.at(atoi(subsplit.at(2).c_str()) - 1));

			if (splitLine.size() < 5) continue;
			SplitString(splitLine.at(3), '/', subsplit);

			positionBuffer->add(positions.at(atoi(subsplit.at(0).c_str()) - 1));
			if (texCoordBuffer) texCoordBuffer->add(texCoords.at(atoi(subsplit.at(1).c_str()) - 1));
			if (normalBuffer) normalBuffer->add(normals.at(atoi(subsplit.at(2).c_str()) - 1));
			SplitString(splitLine.at(4), '/', subsplit);
			positionBuffer->add(positions.at(atoi(subsplit.at(0).c_str()) - 1));
			if (texCoordBuffer) texCoordBuffer->add(texCoords.at(atoi(subsplit.at(1).c_str()) - 1));
			if (normalBuffer) normalBuffer->add(normals.at(atoi(subsplit.at(2).c_str()) - 1));
			SplitString(splitLine.at(1), '/', subsplit);
			positionBuffer->add(positions.at(atoi(subsplit.at(0).c_str()) - 1));
			if (texCoordBuffer) texCoordBuffer->add(texCoords.at(atoi(subsplit.at(1).c_str()) - 1));
			if (normalBuffer) normalBuffer->add(normals.at(atoi(subsplit.at(2).c_str()) - 1));
		}
	}

	SetBuffer("in_Position", positionBuffer);
	//for (int x = 0; x < positions.size(); x += 3)
	//{
	//	V1 = positions[x];
	//	V2 = positions[x + 1];
	//	V3 = positions[x + 2];    // for TBN

	//	UV1 = texCoords[x];
	//	UV2 = texCoords[x + 1];
	//	UV3 = texCoords[x + 2];

	//	glm::vec3 deltaPos1 = V2 - V1;
	//	glm::vec3 deltaPos2 = V3 - V1;

	//	glm::vec2 deltaUV1 = UV2 - UV1;
	//	glm::vec2 deltaUV2 = UV3 - UV1;

	//	float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
	//	glm::vec3 Tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
	//	glm::vec3 Bi = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;

	//	tangent.push_back(Tangent);
	//	BiTangent.push_back(Bi);
	//}
	if (texCoordBuffer) SetBuffer("in_TexCoord", texCoordBuffer);
	if (normalBuffer) SetBuffer("in_Normal", normalBuffer);	
	if (tangentBuffer) SetBuffer("in_Tangent", tangentBuffer);
	if (BiTangentBuffer) SetBuffer("in_BiTangent", BiTangentBuffer);
}

void VertexArray::SetBuffer(std::string attribute, VertexBuffer* buffer)
{
	if (attribute == "in_Position")
	{
		buffers.at(0) = buffer;
	}
	else if (attribute == "in_TexCoord")
	{
		buffers.at(1) = buffer;
	}
	else if (attribute == "in_Normal")
	{
		buffers.at(2) = buffer;
	}
	else if (attribute == "in_Tangent")
	{
		buffers.at(3) = buffer;
	}
	else if (attribute == "in_BiTangent")
	{
		buffers.at(4) = buffer;
	}
	else
	{
		throw std::exception();
	}

	dirty = true;
}

int VertexArray::GetVertexCount()
{
	if (!buffers.at(0))
	{
		throw std::exception();
	}

	return buffers.at(0)->GetDataSize() / buffers.at(0)->GetComponet();
}

GLuint VertexArray::GetId()
{
	if (dirty)
	{
		glBindVertexArray(id);

		for (int i = 0; i < buffers.size(); i++)
		{
			if (buffers.at(i))
			{
				glBindBuffer(GL_ARRAY_BUFFER, buffers.at(i)->GetId());
				glVertexAttribPointer(i,
					buffers.at(i)->GetComponet(), GL_FLOAT, GL_FALSE,
					buffers.at(i)->GetComponet() * sizeof(GLfloat), (void *)0);
				glEnableVertexAttribArray(i);
			}
			else
			{
				glDisableVertexAttribArray(i);
			}
		}
	}

	return id;
}

VertexArray::VertexArray()
{
	dirty = false;

	buffers.resize(10);

	glGenVertexArrays(1, &id);

	if (!id)
	{
		throw std::exception();
	}
}


VertexArray::~VertexArray()
{
}
