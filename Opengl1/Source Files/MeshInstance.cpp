
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include "MeshInstance.h"
MeshInstance::MeshInstance(std::vector<Transform>& t_List):transformList(t_List)
{
	//initialize all values to zero or null
	VBO = 0;
	VAO = 0;
	IBO = 0;
	VBOinstance = 0;
	numOfIndices = 0;
	numOfVertices = 0;
	createMat4FromTransform();
	updateTransformBuffer();

}

MeshInstance::~MeshInstance()
{

	clearMesh();
}



void MeshInstance::renderMesh()
{
	glad_glPointSize(2);
	glad_glLineWidth(1);
	//if all the buffers were successfully generated, then render the mesh
	if ((!IBO && numOfIndices) || !VBO || !VAO)
	{
		std::cout << "can't render";
		return;
	}
	glBindVertexArray(VAO);
	if (numOfIndices)
	{
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElementsInstanced(GL_TRIANGLES, numOfIndices, GL_UNSIGNED_INT,nullptr,transformList.size());
	}
	else
	{
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArraysInstanced(GL_TRIANGLES, 0, numOfVertices, transformList.size());
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void MeshInstance::updateTransformBuffer()
{
	createMat4FromTransform();
	glBindBuffer(GL_ARRAY_BUFFER,VBOinstance);
	glBufferSubData(GL_ARRAY_BUFFER,0,transformList.size()*sizeof(glm::mat4),&modelMatList[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void MeshInstance::clearMesh()
{
	//clear mesh from GPU memory and delte all generated buffers
	if (IBO)
		glDeleteBuffers(1, &IBO);
	if (VBO)
		glDeleteBuffers(1, &VBO);
	if (VAO)
		glDeleteVertexArrays(1, &VAO);

	//reset the number of IBO indices
	numOfIndices = 0;
	numOfVertices = 0;
}

void MeshInstance::createMat4FromTransform()
{
	//generate 4x4 matrix to store it in a vertex object buffer
	//this is performance intensive for large number of transforms
	modelMatList.clear();
	for (int i = 0; i < transformList.size(); i++)
	{
		modelMatList.push_back(transformList[i].transformMatrix());
	}
}

void MeshInstance::createMesh(const std::vector<vert>& vertices, const std::vector<GLuint>& indices)
{

	//generate all buffers required for rendering and storing the mesh on the GPU

	//generate Vertex Buffer object
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

	//generate Index Buffer Object
	if (indices.size() > 0)
	{
		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);
	}

	//generate Vertex Attribute Object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//vertex coordinate
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vert), (void*)(offsetof(struct vert, pos)));
	glEnableVertexAttribArray(0);

	//vertex normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vert), (void*)(offsetof(struct vert, normal)));
	glEnableVertexAttribArray(1);

	//texture coordinate
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vert), (void*)(offsetof(struct vert, uv)));
	glEnableVertexAttribArray(2);


	//unBinding all the buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



	//Creating a VBO to store 
	glGenBuffers(1, &VBOinstance);
	glBindBuffer(GL_ARRAY_BUFFER, VBOinstance);
	glBufferData(GL_ARRAY_BUFFER, transformList.size() * sizeof(glm::mat4), &modelMatList[0], GL_STATIC_DRAW);



	//setting attribite pointer for each row of matrices with 4 component in each row
	long long  vecSize = sizeof(glm::vec4);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, vecSize * 4, (const void*)(0));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, vecSize * 4, (const void*)(1 * vecSize));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, vecSize * 4, (const void*)(2 * vecSize));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, vecSize * 4, (const void*)(3 * vecSize));
	glEnableVertexAttribArray(6);

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);

	numOfIndices = indices.size();
	numOfVertices = vertices.size();
	if ((!IBO && numOfIndices) || !VBO || !VAO)
		std::cout << "Error creating Mesh!\n";


}



