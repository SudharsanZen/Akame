#pragma once
#include"Core/AkameCore.h"


class AKAME_API ShadowFBO
{
private:
	unsigned int frameBuffer;
	unsigned int outDepth;
	int height, width;
public:

	ShadowFBO(int height, int width)
	{
		frameBuffer = 0;
		outDepth = 0;
		this->height = height;
		this->width=width;
		initFBO(height,width);
	}

	void initFBO(int height, int width);

	void bindShadowBuffer();
	void unBindShadowBuffer();
	unsigned int getMapBuff() { return outDepth; }
	void useDepthTexture(unsigned int textureUnit);
	int getWidth() { return width; }
	int getHeight() { return height; }
	~ShadowFBO();
};