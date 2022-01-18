#pragma once
#include<string>
#include"Components/Rendering/Mesh.h"
#include"Components/Animation/SkeletalMesh.h"
#include<map>

struct aiMesh;
struct aiNode;
struct aiScene;
class Scene;
/***************************************************************
 * class is used for importing models from files into the scene*
 * constructor takes one argument of the current scene		   *
 ***************************************************************/
class Model
{
private:
	Scene&							mCurrScene;
	const aiScene*					mAiScene;		

	std::string						mDir;			//directory on which the file resides
	std::string						mDirectory;		//file path of model
	
	aiNode*							mRootBoneNode;	//root bone if the imported model has skeletons
	std::map<aiNode*, Entity>		mAllNodeMap;	//map of all aiScene node's pointer to corresponding entity in scene, usefull for identifying bones
	std::map<std::string, aiNode*>	mBoneNodeMap;	//bone name to bone's aiNode map, used later for referencing entities from allNodeMap
	std::map<std::string, BoneInfo>	mBoneMap;		//bone name to boneInfo map
	std::vector<Entity>				mSkMeshList;	//list of skeletal mesh entities if exists

	
	void	processNode(Entity parent, aiNode* node);			//process each node frommAiScene and create an entity for that node in mScene
	Entity	processSkeletalMesh(Entity parent, aiMesh* mesh);
	Entity	processMesh(Entity parent, aiMesh* mesh);

	//this code should be removed (rendered useless)
	void	UpdateHierarchy(aiNode *rootNode);

public:

	//requires current scene as parameter
	Model(Scene& s);

	//loads model from the given "modelPath"
	Entity	LoadModelToScene(std::string modelPath);

};

