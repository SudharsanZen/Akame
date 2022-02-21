#pragma once
#include<vector>
#include"Math/GlmMath.h"
#include"Components/Components.h"
#include"Math/EngineMath.h"
#include<map>
#include"Core/Reflection/ReflectionMeta.h"
#include"Core/AkameCore.h"
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
struct BoneInfo
{
	int				id=-1;					//bone index
	Entity			eid=INVALID_ENTITY;		//entity id of this bone
	std::string		name;					//bone name
	std::string		parentName;				//parent bone's name, left "" ((i.e) null string) if no parents exist
	

	glm::vec3		pose;					//needs to be removed
	glm::quat		rot;					//needs to be removed
	glm::vec3		scale;					//needs to be removed
	
	glm::mat4		offsetMat;				//offsetMatrix to convert vertex configurations from mesh to bone's local space
	AK_SERIALIZABLES
		(
			AK_ID(id)
			AK_ID_COMPX(eid)
			AK_ID(name)
			AK_ID(parentName)
			AK_ID_COMPX(pose)
			AK_ID_COMPX(rot)
			AK_ID_COMPX(scale)
			AK_ID_COMPX(offsetMat)
		)
};


class SkeletalMesh:public Components
{
private:
	/*******************************************************************
	* SkeletalMesh: contains all the data of all the skeletal meshes
	* -creates and manages single VAO,VBO and IBO for all skeletal meshes 
	********************************************************************/

	static	bool							needsUpdate;		//is set to true if the vertex buffer needs to be updated
	static	std::vector<sk_vert>			vertexData;			//global vertex list
	static	std::vector<unsigned int>		indexList;			//global index list
	static	unsigned int					VAO; 		//Buffer object and Atrribute object iDs
	static	unsigned int					VBO;		//Buffer object and Atrribute object iDs
	static	unsigned int					IBO;		//Buffer object and Atrribute object iDs


	
	size_t		numOfIndices, numOfVertices;//number of IBO indices and vertices
	size_t		start_i, end_i, count, maxInd, minInd;
	glm::vec4	max, min;	//max and min poses in xyz axis, used to calculate bounding boxes

	friend class RenderingSystem;
	friend class Editor;
	friend class AnimationSystem;
	friend class SkeletalMeshRenderingSystem;
	friend class Model;
	friend class ModelExporter;
	//only called when "needsUpdate" is true
	//updates VBO and IBO when needed
	AKAME_API static  void setupMesh();

	//entity id of animController that holds all the animation and bone information
	Entity animController;
	bool m_isModel;
	std::string m_model_path;
	friend class SceneDeserializer;
public:
	
#if defined(AK_PRIVATE_GETTER_SETTER) || defined(AK_EXPORT) 
	AKAME_API std::vector<sk_vert>& getVertData();
	AKAME_API std::vector<unsigned int>& getIndexData();
	AKAME_API bool IsModel();
	AKAME_API void IsModel(bool isModel);
	AKAME_API void SetModelPath(std::string modelPath);
	AKAME_API void SetAnimControllerID(Entity eid);
#endif
	AKAME_API Entity GetAnimControllerID();
	AKAME_API SkeletalMesh();

	AKAME_API ~SkeletalMesh();
	AKAME_API static unsigned int getVAO();
	AKAME_API void reset();
	AKAME_API void renderMesh() const;

	//initialize mesh with vertex and index data
	AKAME_API void CreateMesh(std::vector<sk_vert>& vertices, std::vector<unsigned int>& indices = std::vector<unsigned int>());
};

