#include "Rendering/Model.h"
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include"misc/temp.h"
#include"Components/Animation/SkeletalMesh.h"
#include"Animation/AnimationControllerSystem.h"
#include<assimp/version.h>

Entity Model::processSkeletalMesh(Entity parent,aiMesh* mesh)
{
	
	std::set<std::string> bonesNames;
	std::vector<sk_vert> vertices;
	std::vector<unsigned int> indices;
	std::string meshName = mesh->mName.C_Str();
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		sk_vert vertex;
		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;

		vertex.normal.x = mesh->mNormals[i].x;
		vertex.normal.y = mesh->mNormals[i].y;
		vertex.normal.z = mesh->mNormals[i].z;
		vertex.boneWeight.x = 0;
		vertex.boneWeight.y = 0;
		vertex.boneWeight.z = 0;
		vertex.boneWeight.w = 0;

		vertex.boneIndex.x = -1;
		vertex.boneIndex.y = -1;
		vertex.boneIndex.z = -1;
		vertex.boneIndex.w = -1;

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
	

	Entity meshid = currScene.CreateEntity();
	skMeshList.push_back(meshid);
	Transform& t = currScene.AddComponent<Transform>(meshid);
	t.setParent(parent);
	t.SetLocalScale(glm::vec3(1,1,1));
	t.SetLocalPosition(glm::vec3(0,0,0));
	t.SetLocalRotation(glm::quat(1,0,0,0));
	currScene.SetEntityName(meshid, meshName);
	for (int i_bone = 0; i_bone < mesh->mNumBones; i_bone++)
	{
		int boneId = -1;
		auto &currBone=mesh->mBones[i_bone];
		if (bonesNames.find(currBone->mName.C_Str())==bonesNames.end())
		{
			bonesNames.insert(currBone->mName.C_Str());
			
		}
		if (boneMap.find(currBone->mName.C_Str()) == boneMap.end())
		{
			
			Bone b;
			b.id = boneMap.size();
			b.name = currBone->mName.C_Str();
			
			aiVector3D pose;
			aiVector3D scale;
			aiQuaternion rot;
			ai_real angle;
			aiVector3D angles;
			boneId = b.id;
			currBone->mArmature->mTransformation.Decompose(scale,rot,pose);
		
			b.scale = glm::vec3(scale.x,scale.y,scale.z);
			b.pose = glm::vec3(pose.x,pose.y,pose.z);
			b.rot = (glm::quat(rot.w,rot.x,rot.y,rot.z));
			//b.rotAxis = glm::vec3(axis.x, axis.y, axis.z);
			//b.rotAngle = angle;
			
		
			b.parentName = "";
			if (currBone->mNode->mParent != nullptr)
			{
				b.parentName=currBone->mNode->mParent->mName.C_Str();	
			}


			boneNodeMap[b.name] = currBone->mNode;

			boneMap[b.name] = b;
		}
		else
		{
			boneId = boneMap[currBone->mName.C_Str()].id;
		}
		assert(boneId!=-1);
		auto weights = currBone->mWeights;
		for (int i_weight = 0; i_weight < currBone->mNumWeights; i_weight++)
		{
			int vertexId = weights[i_weight].mVertexId;
			float weight = weights[i_weight].mWeight;
			assert(vertexId<=vertices.size());
			for (int i = 0; i < 4; i++)
			{
				if (vertices[vertexId].boneIndex[i] == -1)
				{
					vertices[vertexId].boneIndex[i] = boneId;
					vertices[vertexId].boneWeight[i]= weight;
					break;
				}
			}
		}
	}



	
	SkeletalMesh &m=currScene.AddComponent<SkeletalMesh>(meshid);
	m.boneMap=std::vector<Bone>(bonesNames.size());

	for (auto bStr : bonesNames)
	{
		assert(boneMap.find(bStr) != boneMap.end());
		{
			auto& bone = boneMap[bStr];
			m.boneMap[bone.id]=bone;
		}

	}
	std::vector<sk_vert> finalVert;
	for (size_t i = 0; i < indices.size(); i += 3)
	{
		sk_vert v1, v2, v3;
		v1 = vertices[indices[i]];
		v2 = vertices[indices[i + 1]];
		v3 = vertices[indices[i + 2]];

		calTangentBiTangent(v1, v2, v3);
		finalVert.push_back(v1);
		finalVert.push_back(v2);
		finalVert.push_back(v3);
	}
	m.CreateMesh(finalVert);
	return meshid;
}
Entity Model::processMesh(Entity parent,aiMesh* mesh)
{
	if (mesh->HasBones())
		return processSkeletalMesh(parent,mesh);
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
	t.SetLocalScale(glm::vec3(1, 1, 1));
	t.SetLocalPosition(glm::vec3(0, 0, 0));
	t.SetLocalRotation(glm::quat(1, 0, 0, 0));
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
			mat.setValue("metallic", 0.0f);
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

void Model::processNode(Entity parent,aiNode* node)
{

	Entity currNode;
	/*
	if (node->mNumMeshes <= 1 && node->mNumChildren==0)
	{
		currNode = parent;
	}
	else*/
	{
		
		currNode=currScene.CreateEntity();
		allNodeMap[node] = currNode;
		Transform& t = currScene.AddComponent<Transform>(currNode);
		t.setParent(parent);
		aiVector3D pose;
		aiVector3D scale;
		aiQuaternion quat;

		node->mTransformation.Decompose(scale, quat, pose);
		glm::vec3 p = glm::vec3(pose.x, pose.y, pose.z);
		glm::quat a = glm::quat(quat.w,quat.x,quat.y,quat.z);
		t.SetLocalPosition(p);
		t.SetLocalRotation(Quaternion(a));
		t.SetLocalScale(glm::vec3(scale.x,scale.y,scale.z));
		currScene.SetEntityName(currNode, node->mName.C_Str());
	}
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		
		processMesh(currNode,mesh);
		
	}

	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		processNode(currNode,node->mChildren[i]);
	}
}
Entity Model::LoadModelToScene(std::string modelPath)
{

	unsigned int importOptions = aiProcess_Triangulate
		| aiProcess_JoinIdenticalVertices
		| aiProcess_Triangulate
		|aiProcess_PopulateArmatureData

		;

	Assimp::Importer importer;
	scene = importer.ReadFile(modelPath, importOptions);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return Entity(-1,-1);
	}
	dir = modelPath.substr(0, modelPath.find_last_of('/'))+"/";
	Entity parent=currScene.CreateEntity();
	currScene.AddComponent<Transform>(parent);

	processNode(parent,scene->mRootNode);


	//ignore these Mr._C , these are some useless code that doesn't affect the scene hierarchy and transforms
	aiNode* rootBone;
	for (auto &bonePair : boneMap)
	{
		auto& bone = bonePair.second;
		if (boneNodeMap.find(bone.name) != boneNodeMap.end())
		{
			aiNode *currBoneNode = boneNodeMap[bone.name];
			if (boneNodeMap.find(currBoneNode->mParent->mName.C_Str()) == boneNodeMap.end())
			{
				rootBone = currBoneNode->mParent;;
			}

			if (allNodeMap.find(boneNodeMap[bone.name]) != allNodeMap.end())
			{
				Entity currBone = allNodeMap[boneNodeMap[bone.name]];
				bone.eid = currBone;
			}
			else
			{
				std::cout << "Can't find node "<<bone.name<<" in the scene\n";
			}
		}
		else
		{
			std::cout <<"can't find bone in bone list: "<<bone.name<<std::endl;
		}
			Transform& bT = currScene.GetComponent<Transform>(bone.eid);
			
		
	}

	for (auto& ent : skMeshList)
	{
		
		SkeletalMesh& skm = currScene.GetComponent<SkeletalMesh>(ent);
		for (int i = 0; i < skm.boneMap.size(); i++)
		{
			assert(boneMap.find(skm.boneMap[i].name)!=boneMap.end());
			skm.boneMap[i].eid = boneMap[skm.boneMap[i].name].eid;
		}

	}
	if (boneMap.size() > 0)
	{
		AnimationController& anim = currScene.AddComponent<AnimationController>(parent);
		anim.boneList = std::make_shared<std::vector<Bone>>(boneMap.size());
		anim.boneMap = std::make_shared<std::map<std::string, Bone>>();
		(*anim.boneMap) = boneMap;
		for (auto& bone : boneMap)
		{
			(*anim.boneList)[bone.second.id] = bone.second;
		}
		if (scene->HasAnimations())
		{ 
			unsigned int animCount=scene->mNumAnimations;
			auto animList = scene->mAnimations;

			for (unsigned int i = 0; i < animCount; i++)
			{
				std::cout << animList[i]->mName.C_Str();
			}
		}
	}
	
	//UpdateHierarchy(rootBone);
	return parent;
}
void Model::UpdateHierarchy(aiNode* rootNode)
{
	if (!rootNode)
		return;
	std::string boneName = rootNode->mName.C_Str();
	if (boneMap.find(boneName) != boneMap.end())
	{
		Transform& t = currScene.GetComponent<Transform>(boneMap[boneName].eid);
		auto bone = boneMap[boneName];
	
		t.SetLocalPosition(bone.pose);
		t.SetLocalRotation(bone.rot);
		t.SetLocalScale(bone.scale);
		
	}
	for (int i = 0; i < rootNode->mNumChildren; i++)
	{
		UpdateHierarchy(rootNode->mChildren[i]);
	}
}