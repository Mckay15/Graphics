#include "ShaderProgram.h"
#include "VertexArray.h"
#include "Texture.h"

#include <glm/ext.hpp>

#include <fstream>
#include <iostream>

ShaderProgram::ShaderProgram()
{
	std::string vertShader;
	std::string fragShader;

	std::ifstream file("../shaders/simple.vert");

	if (!file.is_open())
	{
		throw std::exception();
	}
	else
	{
		while (!file.eof())
		{
			std::string line;
			std::getline(file, line);
			vertShader += line + "\n";
		}
	}
	file.close();

	file.open("../shaders/simple.frag");

	if (!file.is_open())
	{
		throw std::exception();
	}
	else
	{
		while (!file.eof())
		{
			std::string line;
			std::getline(file, line);
			fragShader += line + "\n";
		}
	}
	file.close();

	const char *vertex = vertShader.c_str();
	const char *fragment = fragShader.c_str();

	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, &vertex, NULL);
	glCompileShader(vertexShaderId);
	GLint success = 0;
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		throw std::exception();
	}

	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, &fragment, NULL);
	glCompileShader(fragmentShaderId);
	success = 0;
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		throw std::exception();
	}
	id = glCreateProgram();
	glAttachShader(id, vertexShaderId);
	glAttachShader(id, fragmentShaderId);

	glBindAttribLocation(id, 0, "in_Position");

	glBindAttribLocation(id, 1, "in_Texture");

	glLinkProgram(id);
	success = 0;
	glGetProgramiv(id, GL_LINK_STATUS, &success);

	if (!success)
	{
		throw std::exception();
	}
	glDetachShader(id, vertexShaderId);
	glDeleteShader(vertexShaderId);
	glDetachShader(id, fragmentShaderId);
	glDeleteShader(fragmentShaderId);
}
void ShaderProgram::printShaderInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten = 0;
	char *infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		free(infoLog);
	}
}

void ShaderProgram::printProgramInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten = 0;
	char *infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		free(infoLog);
	}
}

ShaderProgram::ShaderProgram(std::string vert, std::string frag)
{
	std::string vertShader;
	std::string fragShader;

	std::ifstream file(vert.c_str());

	if (!file.is_open())
	{
		throw std::exception();
	}
	else
	{
		while (!file.eof())
		{
			std::string line;
			std::getline(file, line);
			vertShader += line + "\n";
		}
	}
	file.close();

	file.open(frag);

	if (!file.is_open())
	{
		throw std::exception();
	}
	else
	{
		while (!file.eof())
		{
			std::string line;
			std::getline(file, line);
			fragShader += line + "\n";
		}
	}
	file.close();

	const char *vertex = vertShader.c_str();
	const char *fragment = fragShader.c_str();

	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, &vertex, NULL);
	glCompileShader(vertexShaderId);
	GLint success = 0;
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		printShaderInfoLog(vertexShaderId);
		printProgramInfoLog(vertexShaderId);
		throw std::exception();
	}

	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, &fragment, NULL);
	glCompileShader(fragmentShaderId);
	success = 0;
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		printShaderInfoLog(fragmentShaderId);
		printProgramInfoLog(fragmentShaderId);
		throw std::exception();
	}
	id = glCreateProgram();
	glAttachShader(id, vertexShaderId);
	glAttachShader(id, fragmentShaderId);

	glBindAttribLocation(id, 0, "in_Position");
	glBindAttribLocation(id, 1, "in_TexCoord");
	glBindAttribLocation(id, 2, "in_Normal");
	glBindAttribLocation(id, 3, "in_Tangent");
	glBindAttribLocation(id, 4, "in_BiTangent");

	if (glGetError() != GL_NO_ERROR)
	{
		throw std::exception();
	}

	glLinkProgram(id);
	success = 0;
	glGetProgramiv(id, GL_LINK_STATUS, &success);

	if (!success)
	{
		throw std::exception();
	}
	glDetachShader(id, vertexShaderId);
	glDeleteShader(vertexShaderId); 
	glDetachShader(id, fragmentShaderId); 
	glDeleteShader(fragmentShaderId);
}

//void ShaderProgram::draw(VertexArray *vertexArray)
//{
//
//}

void ShaderProgram::draw(VertexArray * vertexArray)
{
	glUseProgram(id);
	glBindVertexArray(vertexArray->GetId());

	for (size_t i = 0; i < samplers.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		if (samplers.at(i).texture)
		{
			glBindTexture(GL_TEXTURE_2D, samplers.at(i).texture->getId());
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	glDrawArrays(GL_TRIANGLES, 0, vertexArray->GetVertexCount());

	for (size_t i = 0; i < samplers.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glBindVertexArray(0);
	glUseProgram(0);
}

void ShaderProgram::SetUniform(std::string uniform, glm::mat4 value)
{
	GLint uniformId = glGetUniformLocation(id, uniform.c_str());

	if (uniformId == -1)
	{
		throw std::exception();
	}

	glUseProgram(id);
	glUniformMatrix4fv(uniformId, 1, GL_FALSE, &value[0][0]);
	glUseProgram(0);
}
void ShaderProgram::SetUniform(std::string uniform, int value)
{
	GLint uniformId = glGetUniformLocation(id, uniform.c_str());

	if (uniformId == -1)
	{
		throw std::exception();
	}

	glUseProgram(id);
	glUniform1f(uniformId, value);
	glUseProgram(0);
}

void ShaderProgram::SetUniform(std::string uniform, glm::vec4 value)
{
	GLint uniformId = glGetUniformLocation(id, uniform.c_str());

	if (uniformId == -1)
	{
		throw std::exception();
	}

	glUseProgram(id);
	glUniform4f(uniformId, value.x, value.y, value.z, value.w);
	glUseProgram(0);
}

void ShaderProgram::SetUniform(std::string uniform, glm::vec3 value)
{
	GLint uniformId = glGetUniformLocation(id, uniform.c_str());
	
	if (uniformId == -1)
	{
		printShaderInfoLog(uniformId);
		printProgramInfoLog(uniformId);
		throw std::exception();
	}

	glUseProgram(id);
	glUniform3f(uniformId, value.x, value.y, value.z);
	glUseProgram(0);
}

void ShaderProgram::SetUniform(std::string uniform, float value)
{
	GLint uniformId = glGetUniformLocation(id, uniform.c_str());

	if (uniformId == -1)
	{
		throw std::exception();
	}

	glUseProgram(id);
	glUniform1f(uniformId, value);
	glUseProgram(0);
}
void ShaderProgram::SetUniform(std::string uniform, Texture *texture)
{
	GLint uniformId = glGetUniformLocation(id, uniform.c_str());

	if (uniformId == -1)
	{
		throw std::exception();
	}

	for (size_t i = 0; i < samplers.size(); i++)
	{
		if (samplers.at(i).id == uniformId)
		{
			samplers.at(i).texture = texture;

			glUseProgram(id);
			glUniform1i(uniformId, i);
			glUseProgram(0);
			return;
		}
	}

	Sampler s;
	s.id = uniformId;
	s.texture = texture;
	samplers.push_back(s);

	glUseProgram(id);
	glUniform1i(uniformId, samplers.size() - 1);
	glUseProgram(0);
}

void ShaderProgram::UseThis()
{
	glUseProgram(id);
}

GLuint ShaderProgram::getId()
{
	return id;
}

ShaderProgram::~ShaderProgram()
{
}
