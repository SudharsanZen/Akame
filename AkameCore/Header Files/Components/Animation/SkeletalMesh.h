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
struct BoneInfo
{
	int				id;			//bone index
	Entity			eid;		//entity id of this bone
	std::string		name;		//bone name
	std::string		parentName;	//parent bone's name, left "" ((i.e) null string) if no parents exist
	

	glm::vec3		pose;		//needs to be removed
	glm::quat		rot;		//needs to be removed
	glm::vec3		scale;		//needs to be removed
	
	glm::mat4		offsetMat;	//offsetMatrix to convert vertex configurations from mesh to bone's local space
	
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
	static	unsigned int					VAO, VBO, IBO;		//Buffer object and Atrribute object iDs


	
	size_t		numOfIndices, numOfVertices;//number of IBO indices and vertices
	size_t		start_i, end_i, count, maxInd, minInd;
	glm::vec4	max, min;	//max and min poses in xyz axis, used to calculate bounding boxes

	friend class RenderingSystem;
	friend class Editor;
	friend class AnimationSystem;
	friend class SkeletalMeshRenderingSystem;
	friend class Model;

	//only called when "needsUpdate" is true
	//updates VBO and IBO when needed
	static void setupMesh();

	//entity id of animController that holds all the animation and bone information
	Entity animController;

public:
	
	Entity GetAnimControllerID() { return animController; }
	SkeletalMesh();

	~SkeletalMesh();
	static unsigned int getVAO() { return VAO; }
	void reset();
	void renderMesh() const;

	//initialize mesh with vertex and index data
	void CreateMesh(std::vector<sk_vert>& vertices, std::vector<unsigned int>& indices = std::vector<unsigned int>());
};

