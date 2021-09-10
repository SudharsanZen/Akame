#pragma once

#include<vector>
#include"Math/GlmMath.h"
#include"Components/Components.h"
struct vert
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec3 tangent;
	glm::vec3 biTangent;
};

class Mesh:public Components
{
private:
	//Buffer object and Atrribute object iDs
	unsigned int VAO, VBO, IBO;
	//number of IBO indices
	size_t numOfIndices,numOfVertices;
	void setupMesh();
	
public:
	std::vector<vert> vertices;
	std::vector<unsigned int> indices;

	Mesh();

	~Mesh();

	void reset();
	void renderMesh();
	void CreateMesh(std::vector<vert> vertices, std::vector<unsigned int> indices=std::vector<unsigned int>());

};



