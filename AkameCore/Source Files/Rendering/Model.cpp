#include "Rendering/Model.h"
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include"misc/temp.h"
#include"Core/Log/Log.h"
#include"Components/Animation/SkeletalMesh.h"
#include"Animation/AnimationControllerSystem.h"
#include<assimp/version.h>
#include"Core/Scene.h"
void set_material(Material &mat,aiMesh *mesh,const aiScene *mAiScene,std::string mDir)
{
	if (mesh->mMaterialIndex >= 0)
	{
		aiString diff, rough, norm, metallic, ao;
		int difC = 0, roughC = 0, normC = 0, metalC = 0, aoC = 0;
		aiMaterial* material = mAiScene->mMaterials[mesh->mMaterialIndex];
		for (int i = 0; i < (difC = material->GetTextureCount(aiTextureType_DIFFUSE)); i++)
		{
			material->GetTexture(aiTextureType_DIFFUSE, i, &diff);
		}
		for (int i = 0; i < (roughC = material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS)); i++)
		{
			material->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, i, &rough);
		}
		for (int i = 0; i < (normC = material->GetTextureCount(aiTextureType_NORMALS)); i++)
		{
			material->GetTexture(aiTextureType_NORMALS, i, &norm);
		}
		for (int i = 0; i < (metalC = material->GetTextureCount(aiTextureType_METALNESS)); i++)
		{
			material->GetTexture(aiTextureType_METALNESS, i, &metallic);
		}
		for (int i = 0; i < (aoC = material->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION)); i++)
		{
			material->GetTexture(aiTextureType_AMBIENT_OCCLUSION, i, &ao);
		}

		std::string rDir = AssetManager::assetRootPath;
		if (difC)
			mat.setTexture2D("material.diffuse", (mDir + diff.C_Str()).c_str());
		else
			mat.setTexture2D("material.diffuse", rDir + "EngineAssets/defaultDiff.jpg");
		if (roughC)
			mat.setTexture2D("material.roughness", (mDir + rough.C_Str()).c_str());
		else
		{
			mat.setValue("noRoughness", 1.0f);
			mat.setValue("roughness", 0.4f);
		}
		if (normC)
		{
			mat.setTexture2D("material.normalMap", (mDir + norm.C_Str()).c_str());
			mat.setValue("normalStrength", 5);
		}
		else
		{
			mat.setValue("noNormal", 1);
		}

		if (metalC)
			mat.setTexture2D("material.metallic", (mDir + metallic.C_Str()).c_str());
		else
		{
			mat.setValue("noMetallic", 1.0f);
			mat.setValue("metallic", 0.0f);
		}

		if (aoC)
			mat.setTexture2D("material.metallic", (mDir + ao.C_Str()).c_str());
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
}
template <typename real>
void mat4x4_AiToAkame_converter(aiMatrix4x4t<real> const &aiMat,glm::mat4 &akMat)
{
	
	akMat[0][0] = aiMat.a1,akMat[0][1]=aiMat.b1, akMat[0][2] = aiMat.c1, akMat[0][3] = aiMat.d1;
	akMat[1][0] = aiMat.a2,akMat[1][1]=aiMat.b2, akMat[1][2] = aiMat.c2, akMat[1][3] = aiMat.d2;
	akMat[2][0] = aiMat.a3,akMat[2][1]=aiMat.b3, akMat[2][2] = aiMat.c3, akMat[2][3] = aiMat.d3;
	akMat[3][0] = aiMat.a4,akMat[3][1]=aiMat.b4, akMat[3][2] = aiMat.c4, akMat[3][3] = aiMat.d4;


}
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

		vertex.biTangent.x = mesh->mBitangents[i].x;
		vertex.biTangent.y = mesh->mBitangents[i].y;
		vertex.biTangent.z = mesh->mBitangents[i].z;

		vertex.tangent.x = mesh->mTangents[i].x;
		vertex.tangent.y = mesh->mTangents[i].y;
		vertex.tangent.z = mesh->mTangents[i].z;

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
	

	Entity meshid = mCurrScene.CreateEntity();
	mSkMeshList.push_back(meshid);
	Transform& t = mCurrScene.AddComponent<Transform>(meshid);
	t.setParent(parent);
	t.SetLocalScale(glm::vec3(1,1,1));
	t.SetLocalPosition(glm::vec3(0,0,0));
	t.SetLocalRotation(glm::quat(1,0,0,0));
	mCurrScene.SetEntityName(meshid, meshName);
	for (int i_bone = 0; i_bone < mesh->mNumBones; i_bone++)
	{
		int boneId = -1;
		auto &currBone=mesh->mBones[i_bone];
		if (bonesNames.find(currBone->mName.C_Str())==bonesNames.end())
		{
			bonesNames.insert(currBone->mName.C_Str());
			
		}
		if (mBoneMap.find(currBone->mName.C_Str()) == mBoneMap.end())
		{
			
			BoneInfo b;
			b.id = mBoneMap.size();
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
			mat4x4_AiToAkame_converter(currBone->mOffsetMatrix,b.offsetMat);
			
		
			b.parentName = "";
			if (currBone->mNode->mParent != nullptr)
			{
				b.parentName=currBone->mNode->mParent->mName.C_Str();	
			}


			mBoneNodeMap[b.name] = currBone->mNode;

			mBoneMap[b.name] = b;
		}
		else
		{
			boneId = mBoneMap[currBone->mName.C_Str()].id;
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



	
	SkeletalMesh &skMesh=mCurrScene.AddComponent<SkeletalMesh>(meshid);
	Material mat("SkinnedMeshRenderer");
	set_material(mat,mesh,mAiScene,mDir);
	mCurrScene.AddComponent<Material>(meshid)=mat;
	/*m.boneMap = std::vector<BoneInfo>(bonesNames.size());

	for (auto bStr : bonesNames)
	{
		assert(mBoneMap.find(bStr) != mBoneMap.end());
		{
			auto& bone = mBoneMap[bStr];
			m.boneMap[bone.id]=bone;
		}

	}*/
	std::vector<sk_vert> finalVert;
	//
	for (size_t i = 0; i < vertices.size(); i++)
	{
		//rescale vertex weights so that it adds up to one
		float total = vertices[i].boneWeight[0] + vertices[i].boneWeight[1] + vertices[i].boneWeight[2] + vertices[i].boneWeight[3];
		float factor = 1/total;
		vertices[i].boneWeight *= factor;
	}
	
	for (size_t i = 0; i < indices.size(); i += 3)
	{
		sk_vert v1, v2, v3;
		v1 = vertices[indices[i]];
		v2 = vertices[indices[i + 1]];
		v3 = vertices[indices[i + 2]];

		//calTangentBiTangent(v1, v2, v3);
		finalVert.push_back(v1);
		finalVert.push_back(v2);
		finalVert.push_back(v3);
	}
	skMesh.CreateMesh(finalVert);
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
	
	Entity meshid = mCurrScene.CreateEntity();
	Transform &t=mCurrScene.AddComponent<Transform>(meshid);
	t.setParent(parent);
	t.SetLocalScale(glm::vec3(1, 1, 1));
	t.SetLocalPosition(glm::vec3(0, 0, 0));
	t.SetLocalRotation(glm::quat(1, 0, 0, 0));
	mCurrScene.SetEntityName(meshid,meshName);
	Material mat("DEFERRED");
	
	set_material(mat,mesh,mAiScene,mDir);
	
	
	mCurrScene.AddComponent<Material>(meshid) = mat;
	mCurrScene.AddComponent<Mesh>(meshid).CreateMesh(finalVert);
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
		
		currNode=mCurrScene.CreateEntity();
		mAllNodeMap[node] = currNode;
		Transform& t = mCurrScene.AddComponent<Transform>(currNode);
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
		mCurrScene.SetEntityName(currNode, node->mName.C_Str());
	}
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = mAiScene->mMeshes[node->mMeshes[i]];
		
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
		| aiProcess_CalcTangentSpace

		;

	Assimp::Importer importer;
	mAiScene = importer.ReadFile(modelPath, importOptions);

	if (!mAiScene || mAiScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !mAiScene->mRootNode)
	{
		std::string errstr = "ERROR::ASSIMP::" + std::string(importer.GetErrorString());
		ENGINE_CORE_ERROR(errstr);
		return Entity(-1,-1);
	}
	mDir = modelPath.substr(0, modelPath.find_last_of('/'))+"/";
	Entity parent=mCurrScene.CreateEntity();
	mCurrScene.AddComponent<Transform>(parent);

	processNode(parent,mAiScene->mRootNode);


	//finds root bone node, also assigns the bone entity ids "eid" in the BoneInfo of "mBoneMap"
	aiNode* rootBone;
	for (auto &bonePair : mBoneMap)
	{
		auto& bone = bonePair.second;
		if (mBoneNodeMap.find(bone.name) != mBoneNodeMap.end())
		{
			aiNode *currBoneNode = mBoneNodeMap[bone.name];
			if (mBoneNodeMap.find(currBoneNode->mParent->mName.C_Str()) == mBoneNodeMap.end())
			{
				rootBone = currBoneNode->mParent;;
			}

			if (mAllNodeMap.find(mBoneNodeMap[bone.name]) != mAllNodeMap.end())
			{
				Entity currBone = mAllNodeMap[mBoneNodeMap[bone.name]];
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
			Transform& bT = mCurrScene.GetComponent<Transform>(bone.eid);
			
		
	}


	if (mBoneMap.size() > 0)
	{

		AnimationController& anim = mCurrScene.AddComponent<AnimationController>(parent);
		anim.boneList = std::make_shared<std::vector<BoneInfo>>(mBoneMap.size());
		anim.boneMap = std::make_shared<std::map<std::string, BoneInfo>>();
		(*anim.boneMap) = mBoneMap;

		for (auto& ent : mSkMeshList)
		{

			SkeletalMesh& skm = mCurrScene.GetComponent<SkeletalMesh>(ent);
			skm.animController = parent;
			/*for (int i = 0; i < skm.boneMap.size(); i++)
			{
				assert(mBoneMap.find(skm.boneMap[i].name)!=mBoneMap.end());
				skm.boneMap[i].eid = mBoneMap[skm.boneMap[i].name].eid;
			}*/


		}

		for (auto& bone : mBoneMap)
		{
			(*anim.boneList)[bone.second.id] = bone.second;
		}
		if (mAiScene->HasAnimations())
		{ 
			unsigned int animCount=mAiScene->mNumAnimations;
			auto animList = mAiScene->mAnimations;
			std::cout << "animation list:\n";
			for (unsigned int i = 0; i < animCount; i++)
			{

				std::cout << animList[i]->mName.C_Str()<<"\n";
				//testCode:this code only for testA.fbx
				if (i == 0)
				{
					AnimationClip aClip;
					aClip.duration=animList[i]->mDuration;
					aClip.ticksPerSec = animList[i]->mTicksPerSecond;
					aClip.numChannels=animList[i]->mNumChannels;
					auto mChannel = animList[i]->mChannels;
					for (int i_channel = 0; i_channel < aClip.numChannels; i_channel++)
					{
						AnimationClip::AnimKeys animkeys;
						
						//get animation key count
						animkeys.pose_count=animList[i]->mChannels[i_channel]->mNumPositionKeys;
						animkeys.scale_count=animList[i]->mChannels[i_channel]->mNumScalingKeys;
						animkeys.rot_count=animList[i]->mChannels[i_channel]->mNumRotationKeys;

						//get animation keys for this the current node

						//get pose keys
						for (int i_key = 0; i_key < animkeys.pose_count; i_key++)
						{
							auto currKey = mChannel[i_channel]->mPositionKeys[i_key];
							glm::vec3 pose = glm::vec3(currKey.mValue.x,currKey.mValue.y,currKey.mValue.z);
							AnimationClip::Key key(currKey.mTime,pose);
							animkeys.position.push_back(key);
						}

						//get rotation keys
						for (int i_key = 0; i_key < animkeys.rot_count; i_key++)
						{
							auto currKey = mChannel[i_channel]->mRotationKeys[i_key];
							glm::quat rot = glm::quat(currKey.mValue.w,currKey.mValue.x, currKey.mValue.y, currKey.mValue.z);
							AnimationClip::Key key(currKey.mTime, rot);
							animkeys.rotation.push_back(key);
						}

						//get scale keys
						for (int i_key = 0; i_key < animkeys.scale_count; i_key++)
						{
							auto currKey = mChannel[i_channel]->mScalingKeys[i_key];
							glm::vec3 scale = glm::vec3(currKey.mValue.x, currKey.mValue.y, currKey.mValue.z);
							AnimationClip::Key key(currKey.mTime, scale);
							animkeys.scale.push_back(key);
						}
						
						//add the created animation keys to the boneName to Keys map in the animation clip
						aClip.boneNameKeysMap[animList[i]->mChannels[i_channel]->mNodeName.C_Str()]= animkeys;

					}

					anim.setCurrentClip(aClip);
					
				}
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
	if (mBoneMap.find(boneName) != mBoneMap.end())
	{
		Transform& t = mCurrScene.GetComponent<Transform>(mBoneMap[boneName].eid);
		auto bone = mBoneMap[boneName];
		
		
		t.SetLocalPosition(bone.pose);
		t.SetLocalRotation(bone.rot);
		t.SetLocalScale(bone.scale);
		
	}
	for (int i = 0; i < rootNode->mNumChildren; i++)
	{
		UpdateHierarchy(rootNode->mChildren[i]);
	}
}

Model::Model(Scene& s) :mCurrScene(s)
{
}
