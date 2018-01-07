#include "TextureHandler.h"
#include "stb_image.h"
#include <cassert>
#include <iostream>

TextureHandler::TextureHandler(const std::string& fileTitle)
{
	int textureWidth, textureHeight, ComponentsCount; //width, height, and no of components of image
	unsigned char* imageData = stbi_load((fileTitle).c_str(), &textureWidth, &textureHeight, &ComponentsCount, 4); //load the image and store the data

	if (imageData == NULL)
	{
		std::cerr << "texture load failed" << fileTitle << std::endl;
	}

	glGenTextures(1, &textureIndex); // number of and address of textures
	glBindTexture(GL_TEXTURE_2D, textureIndex); //bind texture - define type 

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // wrap texture outside width
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // wrap texture outside height

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // linear filtering for minification (texture is smaller than area)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // linear filtering for magnifcation (texture is larger)

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData); //Target, Mipmapping Level, Pixel Format, Width, Height, Border Size, Input Format, Data Type of TextureHandler, Image Data

	stbi_image_free(imageData);
}

TextureHandler::~TextureHandler()
{
	glDeleteTextures(1, &textureIndex); // number of and address of textures
}

void TextureHandler::BindTexture(unsigned int index)
{
	assert(index >= 0 && index <= 31); /// check we are working with one of the 32 textures

	glActiveTexture(GL_TEXTURE0 + index); //set acitve texture index
	glBindTexture(GL_TEXTURE_2D, textureIndex); //type of and texture to bind to index
}
