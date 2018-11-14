#include "Texture.h"
#include "stb_image.h"

Texture::Texture(std::string path)
{
	int w = 0;
	int h = 0;
	int channels = 0;

	unsigned char *data = stbi_load(path.c_str(), &w, &h, &channels, 4);

	if (!data)
	{
		throw std::exception();
	}

	GLuint textureId = 0;
	glGenTextures(1, &textureId);

	if (!textureId)
	{
		throw std::exception();
	}

	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	free(data);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 1);

}

glm::vec2 Texture::getSize()
{
	return size;
}

GLuint Texture::getId()
{
	return id;
}

Texture::~Texture()
{
}
