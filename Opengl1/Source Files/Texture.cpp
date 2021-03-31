#include "Texture.h"

Texture::Texture(std::string dir,GLuint imageFormat)
{
	imageDir=dir;
	imageData = nullptr;
	textureID = -1;
	Height = 0;
	Width = 0;
	texFormat=imageFormat;
}


void Texture::clearTextures()
{
	if (textureID)
	{

		glDeleteTextures(1, &textureID);
		textureID = 0;
		Width = 0;
		Height = 0;
		
	}
}

void Texture::loadImage()
{
	clearTextures();
	//generate Texture Buffer and bind it
	glGenTextures(1,&textureID);
	glBindTexture(GL_TEXTURE_2D,textureID);


	if (!textureID)
	{
		std::cout <<"can't create Texture buffer!\n";
		return;
	}


	//texture filtering object and mip map generation 
	{
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_REPEAT);

	}

	//load image from file location into GLubyte*
	imageData = stbi_load(imageDir.c_str(), &Width, &Height, &nrChannels, 0);

	if(imageData)
	{
		glTexImage2D(GL_TEXTURE_2D,0,texFormat,Width,Height,0,texFormat,GL_UNSIGNED_BYTE,imageData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Can't load image, a problem occured!\n";
	}

	stbi_image_free(imageData);
	glBindTexture(GL_TEXTURE_2D,0);
}

void Texture::use(GLuint unit)
{
	
	if (textureID)
	{
		if(!(0 <= unit && unit <= 31))
		{
			std::cout << "invalid texture unit!" << std::endl;
			return;
		}
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glActiveTexture(0);
	}
	else
		std::cout << "can't use texture. textureID not initialized!"<<std::endl;
}


Texture::~Texture()
{
	clearTextures();
}