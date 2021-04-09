#pragma once
#include<iostream>
#include"stb_image.h"
#include"glad/glad.h"
#include"GLFW/glfw3.h"


class Texture
{
private:
	
	//image location
	std::string imageDir;

	//texture buffer reference
	GLuint textureID;



	//image height width and number of color channels
	GLint Height, Width ,nrChannels;

	GLubyte* imageData;
	GLuint texFormat;

	void clearTextures();
public:
	void loadImage();
	void use(const GLuint unit);
	Texture() { imageDir = ""; texFormat = -1; }

	Texture& operator =(Texture&& obj) noexcept
	{
		clearTextures();
		textureID = obj.textureID;
		imageDir = obj.imageDir;
		Height = obj.Height;
		Width = obj.Width;
		texFormat = obj.texFormat;

		obj.textureID = -1;
		obj.clearTextures();
		return *this;
	}

	Texture(Texture&& obj) noexcept
	{
		textureID = obj.textureID;
		imageDir =obj.imageDir;
		Height = obj.Height;
		Width = obj.Width;
		texFormat = obj.texFormat;
		obj.textureID = -1;
		obj.clearTextures();
		
	}

	Texture(const Texture& obj)
	{
		textureID = obj.textureID;
		imageDir = obj.imageDir;
		Height = obj.Height;
		Width = obj.Width;
		texFormat = obj.texFormat;
	}
	
	
	Texture(std::string dir,GLuint imageFormat);
	
	
	
	void setDir(std::string dir, GLuint imageFormat) { imageDir = dir; texFormat = imageFormat; }

	~Texture();
};

