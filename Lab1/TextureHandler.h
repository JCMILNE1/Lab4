#pragma once
#include <string>
#include <GL\glew.h>

class TextureHandler
{
public:
	TextureHandler(const std::string& fileName);

	void BindTexture(unsigned int index); // bind upto 32 textures

	~TextureHandler();

protected:
private:

	GLuint textureIndex;
};

