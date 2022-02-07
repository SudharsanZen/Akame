#pragma once
#include"Core/AkameCore.h"


class ShadowFBO
{
private:
	unsigned int frameBuffer;
	unsigned int outDepth;
	int height, width;
public:

	AKAME_API ShadowFBO(int height, int width);

	AKAME_API void initFBO(int height, int width);

	AKAME_API void bindShadowBuffer();
	AKAME_API void unBindShadowBuffer();
	AKAME_API unsigned int getMapBuff();
	AKAME_API void useDepthTexture(unsigned int textureUnit);
	AKAME_API int getWidth() { return width; }
	AKAME_API int getHeight() { return height; }
	AKAME_API ~ShadowFBO();
};