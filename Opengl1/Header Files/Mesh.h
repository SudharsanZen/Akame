#pragma once
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<vector>
#include<glm/glm/glm.hpp>
struct vert
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
};
class Mesh
{
private:
	//Buffer object and Atrribute object iDs
	GLuint VAO, VBO, IBO;
	//number of IBO indices
	GLuint numOfIndices,numOfVertices;
	void setupMesh();
	
public:
	std::vector<vert> vertices;
	std::vector<GLuint> indices;

	Mesh();

	~Mesh();

	void reset();
	void renderMesh();
	void CreateMesh(std::vector<vert> vertices, std::vector<GLuint> indices=std::vector<GLuint>());

};



