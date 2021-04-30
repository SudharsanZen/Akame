#pragma once
#include<iostream>
#include<string>
#include<vector>
#include"Mesh.h"
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
class Model
{
public:
	Model(std::string path)
	{
		loadModel(path);
	}

	void Draw();
private:
	std::vector<Mesh> meshes;
	std::string directory;

	void loadModel(std::string path);
	void processNode(aiNode *node,const aiScene *scene);
	Mesh processMesh(aiMesh *mesh,const aiScene *scene);


};

