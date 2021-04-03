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
		glDrawElementsInstanced(GL_TRIANGLES, numOfIndices, GL_UNSIGNED_INT,nullptr, transformList.size());
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

