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

	//raw image data
	GLubyte *imageData;

	//image height width and number of color channels
	GLint Height, Width ,nrChannels;

	
	GLuint texFormat;

	void clearTextures();
public:
	void loadImage();
	void use(const GLuint unit);
	Texture() { imageDir = ""; texFormat = -1; }
	Texture(std::string dir,GLuint imageFormat);
	void setDir(std::string dir, GLuint imageFormat) { imageDir = dir; texFormat = imageFormat; }
	~Texture();
};

