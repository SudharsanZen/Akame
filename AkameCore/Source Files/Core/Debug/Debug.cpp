#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include "..\..\..\Header Files\Core\Debug\Debug.h"
#include"Math/Quaternion.h"
unsigned int Debug::vao=0;
unsigned int Debug::vbo=0;
std::vector<Debug::_vert> Debug::lineList;
void Debug::FlushDebugInformation()
{
	lineList.clear();
}
void Debug::DrawBB(glm::vec3 min, glm::vec3 max, glm::mat4 transform, glm::vec3 color)
{
	_vert v1, v2, v3, v4, v5, v6, v7, v8;
	v1 = { transform * glm::vec4(max,1),color };
	v2 = { transform * glm::vec4(max.x,min.y,max.z,1),color };
	v3 = { transform * glm::vec4(min.x,min.y,max.z,1),color };
	v4 = { transform * glm::vec4(min.x,max.y,max.z,1),color };

	v5 = { transform * glm::vec4(min,1),color };
	v6 = { transform * glm::vec4(min.x,max.y,min.z,1),color };
	v7 = { transform * glm::vec4(max.x,max.y,min.z,1),color };
	v8 = { transform * glm::vec4(max.x,min.y,min.z,1),color };

	lineList.push_back(v1);
	lineList.push_back(v2);

	lineList.push_back(v2);
	lineList.push_back(v3);

	lineList.push_back(v3);
	lineList.push_back(v4);

	lineList.push_back(v4);
	lineList.push_back(v1);

	lineList.push_back(v5);
	lineList.push_back(v6);

	lineList.push_back(v6);
	lineList.push_back(v7);

	lineList.push_back(v7);
	lineList.push_back(v8);

	lineList.push_back(v8);
	lineList.push_back(v5);

	lineList.push_back(v1);
	lineList.push_back(v7);

	lineList.push_back(v2);
	lineList.push_back(v8);

	lineList.push_back(v3);
	lineList.push_back(v5);

	lineList.push_back(v4);
	lineList.push_back(v6);



}
void Debug::CreateBuffers()
{
	if (!(vbo && vao))
	{
		glGenBuffers(1, &vbo);
		glGenVertexArrays(1, &vao);

	}
	else
	{
		updateBufferContent();
	}

}

void Debug::updateBufferContent()
{
	if (lineList.size() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, lineList.size() * sizeof(lineList[0]), &lineList[0], GL_DYNAMIC_DRAW);
		glBindVertexArray(vao);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(_vert), (void*)(offsetof(struct _vert, point)));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(_vert), (void*)(offsetof(struct _vert, color)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_VERTEX_ARRAY, 0);
		glBindVertexArray(0);
	}
}

void Debug::DrawDebug()
{
	if (lineList.size() > 0 && vao && vbo)
	{
		glBindVertexArray(vao);
		glBindBuffer(GL_VERTEX_ARRAY, vbo);
		glDrawArrays(GL_LINES, 0, lineList.size());
		glBindBuffer(GL_VERTEX_ARRAY, 0);
		glBindVertexArray(0);
	}

}

//call this when the application quits

void Debug::DeleteBuffers()
{
	if (vbo)
	{
		glDeleteBuffers(1, &vbo);
	}
	if (vao)
	{
		glDeleteVertexArrays(1, &vao);
	}
	vao = 0;
	vbo = 0;
}

void Debug::DrawLine(glm::vec3 start, glm::vec3 end, glm::vec3 color)
{
	_vert s = { start,color };
	_vert e = { end,color };
	lineList.push_back(s);
	lineList.push_back(e);

}

void Debug::DrawRay(glm::vec3 origin, glm::vec3 direction, float distance, glm::vec3 color)
{
	DrawLine(origin, origin + direction * distance, color);
}

void Debug::DrawCircle(glm::vec3 origin, glm::vec3 up, float radius, glm::vec3 color, int numOfSegments)
{

	glm::vec3 r1,r2,r;
	up = glm::normalize(up);
	
	r1=glm::cross(up, worldRight)* radius;
	r2=glm::cross(up, normalize(glm::vec3(1)))* radius;
	if (glm::length(r1) > glm::length(r2))
		r = glm::normalize(r1);
	else
		r = glm::normalize(r2);
	float ang = 360.0f / (float)numOfSegments;
	for (int i = 0; i < numOfSegments; i++)
	{
		
		Quaternion rot1 = Quaternion::rotationAroundAxisVector(ang*float(i),up);
		Quaternion rot2 = Quaternion::rotationAroundAxisVector(ang*float(i+1),up);
		_vert v1 = { origin+rot1 * r*radius,color }, v2 = {origin+rot2*r*radius,color};
		lineList.push_back(v1);
		lineList.push_back(v2);
	}
}