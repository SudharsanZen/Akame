#pragma once
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
class Mesh
{
private:
	//Buffer object and Atrribute object iDs
	GLuint VAO, VBO, IBO;
	//number of IBO indices
	GLuint numOfIndices,numOfVertices;

	void clearMesh();
public:
	
	Mesh();
	~Mesh();

	void createMesh(GLfloat* vert, GLuint* index, size_t vertCount, size_t indexCount);
	void renderMesh();
	

};

