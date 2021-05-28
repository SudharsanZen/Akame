#pragma once
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<vector>
#include"transform.h"
#include"Mesh.h"
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

	void createMesh(const std::vector<vert> &vertices,const std::vector<GLuint> &indices);
	void renderMesh();
	void updateTransformBuffer();
	


};



