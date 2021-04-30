#pragma once
#include"Engine.h"
#include<vector>
class lines
{
	GLuint VBO, VAO;
public:
	glm::vec3 color;
	Shader debugShader;
	struct line
	{
		glm::vec3 st;
		glm::vec3 end;
	}vData;
	lines(glm::vec3 st, glm::vec3 end,glm::vec3 color):debugShader("Shaders/DebugShader/default.vert", "Shaders/DebugShader/default.frag")
	{
		this->color = color;
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
	
		debugShader.useShaderProgram();
		debugShader.setUniformVec3("color",color);
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




std::vector<vert> generateSphereVertices(int numSegLat, int numSegLong, float radius, std::vector<GLuint>& ind);
std::vector<vert> generateSphereVertices(int numSegLat, int numSegLong, float radius);
std::vector<vert> generateCubeVertices(int heightSeg=1, int widthSeg=1);
std::vector<vert> generatePlaneVertices(int lengthSeg=1, int widthSeg=1);

