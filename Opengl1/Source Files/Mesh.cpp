#include "Mesh.h"

Mesh::Mesh()
{
	//initialize all values to zero or null
	VBO = 0;
	VAO = 0;
	IBO = 0;
	numOfIndices = 0;
	numOfVertices = 0;
}

Mesh::~Mesh()
{
	
	clearMesh();
}

void Mesh::createMesh(GLfloat* vert, GLuint* index, size_t vertCount, size_t indexCount)
{

	//generate all buffers required for rendering and storing the mesh on the GPU

	//generate Vertex Buffer object
	glGenBuffers(1,&VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,vertCount*sizeof(index[0]),vert,GL_STATIC_DRAW);
	
	//generate Index Buffer Object
	if (indexCount>0)
	{
		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(index[0]), index, GL_STATIC_DRAW);
	}

	//generate Vertex Attribute Object
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);

	//vertex coordinate
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, sizeof(vert[0]) * 8,(void*)(0));
	glEnableVertexAttribArray(0);

	//vertex normal
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(vert[0])*8,(void*)(3*sizeof(vert[0])));
	glEnableVertexAttribArray(1);

	//texture coordinate
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(vert[0])*8,(void*)(6*sizeof(vert[0])));
	glEnableVertexAttribArray(2);

	//unBinding all the buffers
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindVertexArray(0);

	numOfIndices = indexCount;
	numOfVertices = vertCount;
	if ((!IBO && indexCount) || !VBO || !VAO)
		std::cout << "Error creating Mesh!\n";


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


void Mesh::clearMesh()
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
