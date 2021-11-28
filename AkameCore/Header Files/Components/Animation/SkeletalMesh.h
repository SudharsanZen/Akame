#pragma once
#include<vector>
#include"Math/GlmMath.h"
#include"Components/Components.h"
#include"Math/EngineMath.h"
#include<map>
struct sk_vert
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec3 tangent;
	glm::vec3 biTangent;
	glm::vec4 boneWeight;
	glm::ivec4 boneIndex;

};
struct Bone
{
	Entity eid;
	std::string name;
	int id;
	glm::vec3 pose;
	
	glm::quat rot;

	glm::vec3 scale;
	glm::mat4 offsetMat;
	std::string parentName;
};
class SkeletalMesh:public Components
{
private:

	static bool needsUpdate;
	static std::vector<sk_vert> vertexData;
	static std::vector<unsigned int> indexList;
	//Buffer object and Atrribute object iDs
	static unsigned int VAO, VBO, IBO;
	static size_t topStack;

	//number of IBO indices
	size_t numOfIndices, numOfVertices;
	size_t start_i, end_i, count, maxInd, minInd;
	static void setupMesh();
	glm::vec4 max, min;
	friend class RenderingSystem;
	friend class Editor;
	friend class AnimationSystem;
	friend class SkeletalMeshRenderingSystem;

	int GetBoneIdWithName(std::string name)
	{
		for (int i = 0; i < boneMap.size(); i++)
		{
			if (name == boneMap[i].name)
			{
				return i;
			}
		}

		return -1;
	}

public:
	std::vector<Bone> boneMap;

	//all the bones that are in the animation controller 
	std::shared_ptr<std::vector<Bone>> bones;
	SkeletalMesh();

	~SkeletalMesh();
	static unsigned int getVAO() { return VAO; }
	void reset();
	void renderMesh() const;
	void CreateMesh(std::vector<sk_vert>& vertices, std::vector<unsigned int>& indices = std::vector<unsigned int>());
};

