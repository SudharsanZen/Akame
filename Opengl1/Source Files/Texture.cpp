#include "Texture.h"
#include"stb_image.h"
#include"glad/glad.h"
#include"GLFW/glfw3.h"
#include"Log/Log.h"
#include<iostream>
Texture::Texture(std::string dir)
{
	imageDir=dir;
	textureID = -1;
	Height = 0;
	Width = 0;
	imageData = nullptr;
	loadImage();
}


void Texture::reset()
{
	if (textureID!=-1)
	{

		glDeleteTextures(1, &textureID);
		ENGINE_CORE_INFO("Destroying Texture"+imageDir);
		
	}
	textureID = -1;
	Width = 0;
	Height = 0;
}

void Texture::loadImage()
{
	reset();
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
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}

	//load image from file location into GLubyte*
	 imageData = stbi_load(imageDir.c_str(), &Width, &Height, &nrChannels, 0);

	 switch (nrChannels)
	 {
	 case 1:
		 texFormat = GL_RED;
		 break;
	 case 2:
		 texFormat =GL_RG;
		 break;
	 case 3:
		 texFormat = GL_RGB;
		 break;
	 case 4:
		 texFormat = GL_RGBA;
		 break;
	 }
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

void Texture::use(unsigned int unit)
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

	}
	else
		std::cout << "can't use texture. textureID not initialized!"<<std::endl;
}


Texture::~Texture()
{
	reset();
}