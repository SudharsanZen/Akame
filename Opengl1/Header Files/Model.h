#pragma once
#include<string>
#include"Mesh.h"

struct aiMesh;
struct aiNode;
struct aiScene;
class Model
{
public:
	Model(std::string path)
	{
		loadModel(path);
	}

	void Draw();
	std::vector<Mesh> meshes;
private:
	
	std::string directory;

	void loadModel(std::string path);
	void processNode(aiNode *node,const aiScene *scene);
	Mesh processMesh(aiMesh *mesh,const aiScene *scene);


};

