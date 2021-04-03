#pragma once
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<vector>
#include"transform.h"
#include<glm/glm/glm.hpp>
class MeshInstance
{
private:
	//Buffer object and Atrribute object iDs
	GLuint VAO, VBO, IBO,VBOinstance;
	//number of IBO indices
	GLuint numOfIndices, numOfVertices;
	std::vector<Transform>& transformList;
	std::vector<glm::mat4> modelMatList;
	void clearMesh();
	void createMat4FromTransform();
public:

	MeshInstance(std::vector<Transform>&);
	~MeshInstance();
	template<class T, class Q>
	void createMesh(const void* vert, const void* index, GLuint vertCount, GLuint indexCount);
	void renderMesh();
	void updateTransformBuffer();
	


};



template<class T, class Q>
void MeshInstance::createMesh(const void* vert, const void* index, GLuint vertCount, GLuint indexCount)
{

	//generate all buffers required for rendering and storing the mesh on the GPU

	//generate Vertex Buffer object
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(T), vert, GL_STATIC_DRAW);

	//generate Index Buffer Object
	if (indexCount > 0)
	{
		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(Q), index, GL_STATIC_DRAW);
	}

	//generate Vertex Attribute Object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//vertex coordinate
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(T) * 8, (void*)(0));
	glEnableVertexAttribArray(0);

	//vertex normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(T) * 8, (void*)(3 * sizeof(T)));
	glEnableVertexAttribArray(1);

	//texture coordinate
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(T) * 8, (void*)(6 * sizeof(T)));
	glEnableVertexAttribArray(2);


	//unBinding all the buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	


	//Creating a VBO to store 
	glGenBuffers(1, &VBOinstance);
	glBindBuffer(GL_ARRAY_BUFFER, VBOinstance);
	glBufferData(GL_ARRAY_BUFFER, transformList.size() * sizeof(glm::mat4), &modelMatList[0], GL_STATIC_DRAW);



	//setting attribite pointer for each row of matrices with 4 component in each row
	int vecSize=sizeof(glm::vec4);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, vecSize * 4, (void*)(0));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, vecSize * 4, (void*)(1*vecSize));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, vecSize * 4, (void*)(2*vecSize));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, vecSize * 4, (void*)(3*vecSize));
	glEnableVertexAttribArray(6);

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);

	numOfIndices = indexCount;
	numOfVertices = vertCount;
	if ((!IBO && indexCount) || !VBO || !VAO)
		std::cout << "Error creating Mesh!\n";


}



