#pragma once
#include"Engine.h"
#include<vector>
class lines
{
	GLuint VBO, VAO;
public:
	struct line
	{
		glm::vec3 st;
		glm::vec3 end;
	}vData;
	lines(glm::vec3 st, glm::vec3 end)
	{
		//generate all buffers required for rendering and storing the mesh on the GPU
		vData.st = st;
		vData.end = end;
		//generate Vertex Buffer object
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vData), &vData, GL_STATIC_DRAW);

		//generate Vertex Attribute Object
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		//vertex coordinate
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(0));
		glEnableVertexAttribArray(0);

		//unBinding all the buffers
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);


	}

	void renderMesh()
	{
		glad_glPointSize(2);
		glad_glLineWidth(1);
		//if all the buffers were successfully generated, then render the mesh

		glBindVertexArray(VAO);

		glDrawArrays(GL_LINES, 0, 2);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}


	void clearMesh()
	{
		if (VBO)
			glDeleteBuffers(1, &VBO);
		if (VAO)
			glDeleteVertexArrays(1, &VAO);

	}

};
extern GLfloat tvertices[16];
extern GLuint tIndex[3];

extern float plane[32];

extern float planevert[20];

extern GLuint planeIndex[6];

extern float boxvertices[180];
extern float boxNormVertices[288];


struct vert
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
};
std::vector<vert> sphere(int numSegLat, int numSegLong, float radius, std::vector<GLuint>& ind);
std::vector<vert> sphere(int numSegLat, int numSegLong, float radius);
GLfloat* serializeVec(std::vector<vert> v);
