#pragma once

#include<vector>
#include"Components/Rendering/Transform.h"
#include"Components/Rendering/Mesh.h"
#include"Math/GlmMath.h"
class MeshInstance
{
private:
	//Buffer object and Atrribute object iDs
	unsigned int VAO, VBO, IBO,VBOinstance;
	//number of IBO indices
	size_t numOfIndices, numOfVertices;
	std::vector<Transform>& transformList;
	std::vector<glm::mat4> modelMatList;
	AKAME_API void clearMesh();
	AKAME_API void createMat4FromTransform();
public:

	AKAME_API MeshInstance(std::vector<Transform>&);
	AKAME_API ~MeshInstance();

	AKAME_API void createMesh(const std::vector<vert> &vertices,const std::vector<unsigned int > &indices);
	AKAME_API void renderMesh();
	AKAME_API void updateTransformBuffer();
	


};



