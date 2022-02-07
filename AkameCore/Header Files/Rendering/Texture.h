#pragma once
#include<string>
#include"Core/AkameCore.h"

class AKAME_API Texture
{
private:
	
	//image location
	std::string imageDir;

	//texture buffer reference
	unsigned int textureID;



	//image height width and number of color channels
	int Height, Width ,nrChannels;

	unsigned char* imageData;
	unsigned int texFormat;
	friend class ContentBrowser;
	friend class FileOpenSaveDialogBox;
	
public:
	void reset();
	void loadImage();
	void use(const unsigned int unit);
	Texture() { imageDir = ""; texFormat = -1; }

	Texture& operator =(Texture&& obj) noexcept
	{
		reset();
		textureID = obj.textureID;
		imageDir = obj.imageDir;
		Height = obj.Height;
		Width = obj.Width;
		texFormat = obj.texFormat;

		obj.textureID = -1;
		obj.reset();
		return *this;
	}

	Texture& operator =(const Texture& obj)=default;


	
	
	Texture(std::string dir);
	
	
	
	void setDir(std::string dir, unsigned int imageFormat) { imageDir = dir; texFormat = imageFormat; }

	~Texture();
};

