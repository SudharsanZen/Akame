#include "Mesh.h"

Mesh::Mesh()
{
	//initialize all values to zero or null
	VBO = 0;
	VAO = 0;
	IBO = 0;
	numOfIndices = 0;
	
}

Mesh::~Mesh()
{
	
	clearMesh();
}

void Mesh::createMesh(GLfloat* vert, GLuint* index, size_t vertCount, size_t indexCount)
{

	//generate all buffers required for rendering and storing the mesh on the GPU
	glGenBuffers(1,&VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,vertCount*sizeof(index[0]),vert,GL_STATIC_DRAW);
	
	glGenBuffers(1,&IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,indexCount*sizeof(index[0]),index,GL_STATIC_DRAW);

	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, sizeof(vert[0]) * 3,static_cast<void*>(0));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindVertexArray(0);

	numOfIndices = indexCount;
	if (!IBO || !VBO || !VAO)
		std::cout << "Error creating Mesh!\n";


}

void Mesh::renderMesh()
{
	//if all the buffers were successfully generated, then render the mesh
	if (!IBO || !VBO || !VAO)
	{
		std::cout << "can't render";
		return;
	}
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_TRIANGLES,numOfIndices, GL_UNSIGNED_INT, nullptr);
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
}
