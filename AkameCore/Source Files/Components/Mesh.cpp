
#include<iostream>
#include "Components/Rendering/Mesh.h"
#include"Core/Log/Log.h"
#include<filesystem>
#pragma warning(push, 0)
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#pragma warning(pop)
std::vector<vert> Mesh::vertexData = std::vector<vert>();
std::vector<unsigned int> Mesh::indexList = std::vector<unsigned int>();
bool Mesh::needsUpdate=false;
//Buffer object and Atrribute object iDs
unsigned int Mesh::VAO=GL_INVALID_VALUE, Mesh::VBO= GL_INVALID_VALUE, Mesh::IBO= GL_INVALID_VALUE;
size_t Mesh::topStack=0;
bool Mesh::IsModel() 
{ 
	return isModel; 
}
void Mesh::IsModel(bool is_model)
{
	isModel = is_model;
}
AKAME_API void Mesh::SetModelPath(std::string loc)
{
	std::filesystem::path path = loc;
	AK_ASSERT(std::filesystem::is_regular_file(loc) && "invalid file!");
	modelPath = loc;
}
Mesh::Mesh()
{
	//initialize all values to zero or null
	isModel = false;
	min = glm::vec4(-1);
	max = glm::vec4(1);
	count = 0;
	end_i = 0;
	maxInd = 0;
	minInd = 0;
	numOfIndices = 0;
	numOfVertices = 0;
	start_i = 0;
}

Mesh::~Mesh()
{
	//clearMesh();

}

unsigned int Mesh::getVAO() 
{ 
	return VAO; 
}



void Mesh::renderMesh() const
{
	
	//if all the buffers were successfully generated, then render the mesh
	if (IBO== GL_INVALID_VALUE|| VBO==GL_INVALID_VALUE || VAO== GL_INVALID_VALUE)
	{
		AK_ASSERT(true && "buffers were not generated! can't render mesh!");
		return;
	}
	if(count)
	glDrawRangeElements(GL_TRIANGLES,static_cast<GLsizei>(minInd), static_cast<GLsizei>(maxInd), static_cast<GLsizei>(count),GL_UNSIGNED_INT,(const void*)((static_cast<GLuint>(start_i))*sizeof(unsigned int)));



	
}

void Mesh::CreateMesh(std::vector<vert> &vertices, std::vector<GLuint> &indices)
{
	
	//Calculate local Bounding boxes for mesh
	glm::vec3 minPose = vertices[0].pos;
	glm::vec3 maxPose = vertices[0].pos;
	for (int i = 1; i < vertices.size(); i++)
	{
		if (minPose.x > vertices[i].pos.x)
			minPose.x = vertices[i].pos.x;
		if (minPose.y > vertices[i].pos.y)
			minPose.y = vertices[i].pos.y;
		if (minPose.z > vertices[i].pos.z)
			minPose.z = vertices[i].pos.z;

		if (maxPose.x < vertices[i].pos.x)
			maxPose.x = vertices[i].pos.x;
		if (maxPose.y < vertices[i].pos.y)
			maxPose.y = vertices[i].pos.y;
		if (maxPose.z < vertices[i].pos.z)
			maxPose.z = vertices[i].pos.z;
	}
	min = glm::vec4(minPose,1);
	max = glm::vec4(maxPose,1);

	size_t prevVertSize = this->vertexData.size();
	start_i = indexList.size();
	minInd = prevVertSize;
	maxInd = prevVertSize + vertices.size()-1;
	if (indices.size() == 0)
	{
		
		for (int i = 0; i < vertices.size(); i++)
		{
			count++;
			this->vertexData.push_back(vertices[i]);
			this->indexList.push_back(i +static_cast<unsigned int>(prevVertSize));
		}
	}
	else
	{
		for (int i = 0; i < vertices.size(); i++)
		{
			vertexData.push_back(vertices[i]);
		}
		for (int i = 0; i < indices.size(); i++)
		{
			count++;
			indexList.push_back(static_cast<unsigned int>(prevVertSize)+indices[i]);
		}
		
	}





	

	numOfIndices = indices.size();
	end_i = this->indexList.size()- 1;
	numOfVertices = vertices.size();
	needsUpdate = true;
	
}


void Mesh::reset()
{
	//clear mesh from GPU memory and delte all generated buffers
	// (IBO)
	//	glDeleteBuffers(1,&IBO);
	//if (VBO)
	//	glDeleteBuffers(1,&VBO);
	//if (VAO)
	//	glDeleteVertexArrays(1,&VAO);
	//max = glm::vec4(1);
	//min = glm::vec4(-1);
	//reset the number of IBO indices
	//numOfIndices = 0;
	//numOfVertices = 0;
}

void Mesh::setupMesh()
{
	//generate Vertex Attribute Object
	if (VAO == GL_INVALID_VALUE)
	{
		glGenVertexArrays(1, &VAO);
	}
	glBindVertexArray(VAO);
	//generate all buffers required for rendering and storing the mesh on the GPU

	//generate Vertex Buffer object
	if(VBO==GL_INVALID_VALUE)
		glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(vertexData[0]), &vertexData[0], GL_STATIC_DRAW);

	

	//vertex coordinate
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vert), (void*)(offsetof(struct vert, pos)));
	glEnableVertexAttribArray(0);

	//vertex normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vert), (void*)(offsetof(struct vert,normal)));
	glEnableVertexAttribArray(1);

	//texture coordinate
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vert), (void*)(offsetof(struct vert,uv)));
	glEnableVertexAttribArray(2);

	//tangent vector 
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vert), (void*)(offsetof(struct vert, tangent)));
	glEnableVertexAttribArray(3);

	//tangent vector 
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(vert), (void*)(offsetof(struct vert, biTangent)));
	glEnableVertexAttribArray(4);
	//generate Index Buffer Object
	if(IBO == GL_INVALID_VALUE)
		glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexList.size() * sizeof(indexList[0]), &indexList[0], GL_STATIC_DRAW);
	

	glBindVertexArray(0);
	//unBinding all the buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



}

