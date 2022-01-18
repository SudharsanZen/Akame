
#pragma once
#include"Math/EngineMath.h"
#include<vector>

class Debug
{
private:
	struct _vert
	{
		glm::vec3 point;
		glm::vec3 color;
	};
	
	static unsigned int vao;
	static unsigned int vbo;

	static std::vector<_vert> lineList;
	//call this every frame after rendering to clear the list of lines
	static void FlushDebugInformation();
	//intitialize the buffers
	static void CreateBuffers();
	//call this before rendering the whole thing to update the vertex buffer
	static void updateBufferContent();
	//called in Rendering System Every frame to render all the queued lines
	static void DrawDebug();
	//call this when the application quits
	static void DeleteBuffers();

	friend class Window;
	friend class Scene;
	friend class RenderingSystem;
	friend class Editor;
public:
	static void DrawBB(glm::vec3 min, glm::vec3 max, glm::mat4 transform, glm::vec3 color);
	
	static void DrawLine(glm::vec3 start, glm::vec3 end, glm::vec3 color = glm::vec3(1));
	static void DrawRay(glm::vec3 origin, glm::vec3 direction, float distance,glm::vec3 color=glm::vec3(1))
	{
		DrawLine(origin,origin+direction*distance,color);
	}
	static void DrawCircle(glm::vec3 origin, glm::vec3 up, float radius, glm::vec3 color = glm::vec3(1), int numOfSegments = 10);

	
};