#include "Model.h"

void Model::Draw()
{
	for (GLuint i = 0; i < meshes.size(); i++)
	{
		meshes[i].renderMesh();
	}
}

void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path,aiProcess_Triangulate|aiProcess_FlipUVs);

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
	Mesh m;
	m.CreateMesh(vertices,indices);
	return m;
}
