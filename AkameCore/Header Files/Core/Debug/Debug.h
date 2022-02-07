
#pragma once
#include"Math/EngineMath.h"
#include<vector>
#include"Core/AkameCore.h"
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
	AKAME_API static void FlushDebugInformation();
	//intitialize the buffers
	AKAME_API static void CreateBuffers();
	//call this before rendering the whole thing to update the vertex buffer
	AKAME_API static void updateBufferContent();
	//called in Rendering System Every frame to render all the queued lines
	AKAME_API static void DrawDebug();
	//call this when the application quits
	AKAME_API static void DeleteBuffers();

	friend class Window;
	friend class Scene;
	friend class Exporter;
	friend class RenderingSystem;
	friend class Editor;
public:
	AKAME_API static void DrawBB(glm::vec3 min, glm::vec3 max, glm::mat4 transform, glm::vec3 color);
	AKAME_API static void DrawLine(glm::vec3 start, glm::vec3 end, glm::vec3 color = glm::vec3(1));
	AKAME_API static void DrawRay(glm::vec3 origin, glm::vec3 direction, float distance, glm::vec3 color = glm::vec3(1));
	AKAME_API static void DrawCircle(glm::vec3 origin, glm::vec3 up, float radius, glm::vec3 color = glm::vec3(1), int numOfSegments = 10);

	
};