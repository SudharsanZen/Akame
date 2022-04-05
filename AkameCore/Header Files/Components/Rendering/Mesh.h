#pragma once

#include<vector>
#include"Math/GlmMath.h"
#include"Components/Components.h"
#include"Core/AkameCore.h"
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
	static bool needsUpdate;
	static std::vector<vert> vertexData;
	static std::vector<unsigned int> indexList;
	//Buffer object and Atrribute object iDs
	static unsigned int VAO;
	static unsigned int IBO;
	static unsigned int VBO;
	static size_t topStack;

	//number of IBO indices
	size_t numOfIndices,numOfVertices;
	size_t start_i, end_i,count,maxInd,minInd;
	AKAME_API static void setupMesh();
	glm::vec4 max,min;
	friend class RenderingSystem;
	friend class Editor;
	friend class SceneDeserializer;
	friend class ModelExporter;
	friend class SceneHierarchyWindow;
	friend class LightAndShadowConfig;
	
	bool isModel;
	std::string modelPath;
public:
	
#if defined(AK_PRIVATE_GETTER_SETTER) || defined(AK_EXPORT) 
	AKAME_API bool IsModel();
	AKAME_API glm::vec3 GetHalfExtent() { return (max - min) / 2.0f; }
	AKAME_API void IsModel(bool is_model);
	AKAME_API void SetModelPath(std::string loc);
	AKAME_API std::vector<vert>& getVertData() { return vertexData; }
	AKAME_API std::vector<unsigned int>& getIndexData() { return indexList; }
	AKAME_API glm::vec3 GetMin() { return min; }
	AKAME_API glm::vec3 GetMax() { return max; }
#endif
	AKAME_API Mesh();

	AKAME_API ~Mesh();
	AKAME_API static unsigned int getVAO();
	AKAME_API void reset();
	AKAME_API void renderMesh() const;
	AKAME_API void CreateMesh(std::vector<vert> &vertices, std::vector<unsigned int> &indices=std::vector<unsigned int>());

};



