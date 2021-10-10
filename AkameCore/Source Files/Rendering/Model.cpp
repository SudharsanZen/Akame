#include "Rendering/Model.h"
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include"misc/temp.h"
void Model::Draw()
{
	for (GLuint i = 0; i < meshes.size(); i++)
	{
		meshes[i].renderMesh();
	}
}

void Model::loadModel(std::string path)
{
	unsigned int importOptions = aiProcess_Triangulate           
    | aiProcess_JoinIdenticalVertices       
    | aiProcess_Triangulate                         
    | aiProcess_FlipUVs;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path,importOptions);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout <<"ERROR::ASSIMP::"<<importer.GetErrorString()<<std::endl;
		return;
	}
	directory = path.substr(0,path.find_last_of('/'));
	processNode(scene->mRootNode,scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh,scene));
	}

	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i],scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<vert> vertices;
	std::vector<unsigned int> indices;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		vert vertex;
		vertex.pos.x=mesh->mVertices[i].x;
		vertex.pos.y=mesh->mVertices[i].y;
		vertex.pos.z=mesh->mVertices[i].z;

		vertex.normal.x =mesh->mNormals[i].x;
		vertex.normal.y =mesh->mNormals[i].y;
		vertex.normal.z =mesh->mNormals[i].z;
		if (mesh->mTextureCoords[0])
		{
			vertex.uv.x = mesh->mTextureCoords[0][i].x;
			vertex.uv.y = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			vertex.uv.x = 0;
			vertex.uv.y = 0;
		}
		vertices.push_back(vertex);
	}

	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
			
			
		}
	}
	std::vector<vert> finalVert;
	for (size_t i = 0; i < indices.size(); i+=3)
	{
		vert v1, v2, v3;
		v1 = vertices[indices[i]];
		v2 = vertices[indices[i+1]];
		v3 = vertices[indices[i + 2]];
	
		calTangentBiTangent(v1, v2, v3);
		finalVert.push_back(v1);
		finalVert.push_back(v2);
		finalVert.push_back(v3);
	}
	Mesh m;
	m.CreateMesh(finalVert);
	return m;
}




Entity processMesh(Entity parent,Scene &currScene,aiMesh* mesh, const aiScene* scene,std::string dir)
{
	std::vector<vert> vertices;
	std::vector<unsigned int> indices;
	std::string meshName=mesh->mName.C_Str();
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		vert vertex;
		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;

		vertex.normal.x = mesh->mNormals[i].x;
		vertex.normal.y = mesh->mNormals[i].y;
		vertex.normal.z = mesh->mNormals[i].z;
		if (mesh->mTextureCoords[0])
		{
			vertex.uv.x = mesh->mTextureCoords[0][i].x;
			vertex.uv.y = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			vertex.uv.x = 0;
			vertex.uv.y = 0;
		}
		vertices.push_back(vertex);
	}

	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);


		}
	}
	std::vector<vert> finalVert;
	for (size_t i = 0; i < indices.size(); i += 3)
	{
		vert v1, v2, v3;
		v1 = vertices[indices[i]];
		v2 = vertices[indices[i + 1]];
		v3 = vertices[indices[i + 2]];

		calTangentBiTangent(v1, v2, v3);
		finalVert.push_back(v1);
		finalVert.push_back(v2);
		finalVert.push_back(v3);
	}
	
	Entity meshid = currScene.CreateEntity();
	Transform &t=currScene.AddComponent<Transform>(meshid);
	t.setParent(parent);

	currScene.SetEntityName(meshid,meshName);
	Material mat("DEFERRED");
	

	if (mesh->mMaterialIndex >= 0)
	{
		aiString diff,rough,norm,metallic,ao;
		int difC=0, roughC=0, normC=0,metalC=0,aoC=0;
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		for (int i = 0; i < (difC=material->GetTextureCount(aiTextureType_DIFFUSE)); i++)
		{
			material->GetTexture(aiTextureType_DIFFUSE,i,&diff);
		}
		for (int i = 0; i < (roughC=material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS)); i++)
		{
			material->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, i, &rough);
		}
		for (int i = 0; i < (normC=material->GetTextureCount(aiTextureType_DISPLACEMENT)); i++)
		{
			material->GetTexture(aiTextureType_DISPLACEMENT, i, &norm);
		}
		for (int i = 0; i < (metalC= material->GetTextureCount(aiTextureType_METALNESS)); i++)
		{
			material->GetTexture(aiTextureType_METALNESS, i, &metallic);
		}
		for (int i = 0; i < (aoC = material->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION)); i++)
		{
			material->GetTexture(aiTextureType_AMBIENT_OCCLUSION, i, &ao);
		}

		std::string rDir = AssetManager::assetRootPath;
		if (difC)
			mat.setTexture2D("material.diffuse",(dir+diff.C_Str()).c_str() );
		else
			mat.setTexture2D("material.diffuse", rDir + "EngineAssets/defaultDiff.jpg");
		if (roughC)
			mat.setTexture2D("material.roughness", (dir + rough.C_Str()).c_str());
		else
		{
			mat.setValue("noRoughness",1.0f);
			mat.setValue("roughness",0.4f);
		}
		if (normC)
		{
			mat.setTexture2D("material.roughness", (dir + norm.C_Str()).c_str());
			mat.setValue("normalStrength", 5);
		}
		else
		{
			mat.setValue("noNormal", 1);
		}

		if (metalC)
			mat.setTexture2D("material.metallic", (dir + metallic.C_Str()).c_str());
		else
		{
			mat.setValue("noMetallic", 1.0f);
			mat.setValue("metallic", 1);
		}

		if (aoC)
			mat.setTexture2D("material.metallic", (dir + ao.C_Str()).c_str());
		else
		{
			mat.setValue("noAO", 1.0f);
			mat.setValue("ambientocclusion", 1.0f);
		}
		std::cout << "Diffuse: " << diff.C_Str() << std::endl;
		std::cout << "rougness: " << rough.C_Str() << std::endl;
		std::cout << "normal: " << norm.C_Str() << std::endl;
		std::cout << "metallic: " << metallic.C_Str() << std::endl;
		std::cout << "ao: " << ao.C_Str() << std::endl;
	
		mat.setValue("roughness", 1);
	}
	
	currScene.AddComponent<Material>(meshid) = mat;
	currScene.AddComponent<Mesh>(meshid).CreateMesh(finalVert);
	return meshid;
}

void processNode(Entity parent,Scene &currScene,aiNode* node, const aiScene* scene,std::string dir)
{

	Entity currNode;
		
	if (node->mNumMeshes <= 1 && node->mNumChildren==0)
	{
		currNode = parent;
	}
	else
	{
		
		currNode=currScene.CreateEntity();
		Transform& t = currScene.AddComponent<Transform>(currNode);
		t.setParent(parent);
		currScene.SetEntityName(currNode, node->mName.C_Str());
	}
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(currNode,currScene,mesh, scene,dir);
	}

	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		processNode(currNode,currScene,node->mChildren[i], scene,dir);
	}
}
Entity LoadModelToScene(Scene &currScene, std::string modelPath)
{
	unsigned int importOptions = aiProcess_Triangulate
		| aiProcess_JoinIdenticalVertices
		| aiProcess_Triangulate
		;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelPath, importOptions);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return Entity(-1,-1);
	}
	std::string directory = modelPath.substr(0, modelPath.find_last_of('/'))+"/";
	Entity parent=currScene.CreateEntity();
	currScene.AddComponent<Transform>(parent);
	processNode(parent,currScene,scene->mRootNode,scene,directory);
	return parent;
}
