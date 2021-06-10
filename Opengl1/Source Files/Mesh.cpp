
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include "Mesh.h"
Mesh::Mesh()
{
	//initialize all values to zero or null
	VBO = 0;
	VAO = 0;
	IBO = 0;
	
}

Mesh::~Mesh()
{
	//clearMesh();
}



void Mesh::renderMesh()
{
	glad_glPointSize(2);
	glad_glLineWidth(1);
	//if all the buffers were successfully generated, then render the mesh
	if ((!IBO && numOfIndices )|| !VBO || !VAO)
	{
		std::cout << "can't render";
		return;
	}
	glBindVertexArray(VAO);
	if (numOfIndices)
	{
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_TRIANGLES, numOfIndices, GL_UNSIGNED_INT, nullptr);
	}
	else
	{
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES,0,numOfVertices);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::CreateMesh(std::vector<vert> vertices, std::vector<GLuint> indices)
{
	this->vertices = vertices;
	this->indices = indices;

	numOfIndices = indices.size();
	numOfVertices = vertices.size();
	setupMesh();
}


void Mesh::reset()
{
	//clear mesh from GPU memory and delte all generated buffers
	if (IBO)
		glDeleteBuffers(1,&IBO);
	if (VBO)
		glDeleteBuffers(1,&VBO);
	if (VAO)
		glDeleteVertexArrays(1,&VAO);

	//reset the number of IBO indices
	numOfIndices = 0;
	numOfVertices = 0;
}

void Mesh::setupMesh()
{

	//generate all buffers required for rendering and storing the mesh on the GPU

	//generate Vertex Buffer object
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, numOfVertices * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

	//generate Index Buffer Object
	if (numOfIndices > 0)
	{
		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numOfIndices * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);
	}

	//generate Vertex Attribute Object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//vertex coordinate
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vert), (void*)(offsetof(struct vert, pos)));
	glEnableVertexAttribArray(0);

	//vertex normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vert), (void*)(offsetof(struct vert,normal)));
	glEnableVertexAttribArray(1);

	//texture coordinate
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vert), (void*)(offsetof(struct vert,uv)));
	glEnableVertexAttribArray(2);

	//unBinding all the buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	if ((!IBO && numOfIndices) || !VBO || !VAO)
		std::cout << "Error creating Mesh!\n";


}

