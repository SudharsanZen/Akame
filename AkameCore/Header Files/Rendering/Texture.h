#pragma once
#include<string>
#include"Core/AkameCore.h"

class Texture
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
	AKAME_API void reset();
	AKAME_API void loadImage();
	AKAME_API void use(const unsigned int unit);
	AKAME_API Texture();

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

	AKAME_API Texture& operator =(const Texture& obj)=default;


	
	
	AKAME_API Texture(std::string dir);
	
	
	
	AKAME_API void setDir(std::string dir, unsigned int imageFormat);

	AKAME_API ~Texture();
};

