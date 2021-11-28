#pragma once
#include<string>
#include"Components/Rendering/Mesh.h"
#include"Components/Animation/SkeletalMesh.h"
#include<map>
struct aiMesh;
struct aiNode;
struct aiScene;


class Model
{
private:
	Scene& currScene;
	const aiScene* scene;
	std::string dir;
	std::map<std::string, Bone> boneMap;
	std::string directory;

	std::map<aiNode*, Entity> allNodeMap;
	std::map<std::string, aiNode*> boneNodeMap;
	std::vector<Entity> skMeshList;
	void processNode(Entity parent, aiNode* node);
	Entity processSkeletalMesh(Entity parent, aiMesh* mesh);
	Entity processMesh(Entity parent, aiMesh* mesh);
	void UpdateHierarchy(aiNode *rootNode);
public:
	Model(Scene &s):currScene(s)
	{
	}

	Entity LoadModelToScene(std::string modelPath);

};

