#pragma once



class ShadowFBO
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

	void useDepthTexture(unsigned int textureUnit);

	~ShadowFBO();
};