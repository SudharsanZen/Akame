#pragma once

#include<vector>
#include"Components/Rendering/Transform.h"
#include"Components/Rendering/Mesh.h"
#include"Math/GlmMath.h"
class AKAME_API MeshInstance
{
private:
	//Buffer object and Atrribute object iDs
	unsigned int VAO, VBO, IBO,VBOinstance;
	//number of IBO indices
	size_t numOfIndices, numOfVertices;
	std::vector<Transform>& transformList;
	std::vector<glm::mat4> modelMatList;
	void clearMesh();
	void createMat4FromTransform();
public:

	MeshInstance(std::vector<Transform>&);
	~MeshInstance();

	void createMesh(const std::vector<vert> &vertices,const std::vector<unsigned int > &indices);
	void renderMesh();
	void updateTransformBuffer();
	


};



