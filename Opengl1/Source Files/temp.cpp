#include"temp.h"
#include<glad/glad.h>



std::vector<vert> generateSphereVertices(int numSegLat, int numSegLong, float radius, std::vector<GLuint>& ind)
{
	std::vector<GLuint> index;
	std::vector<vert> sphere;
	float angleLat = 180.0f / (float)numSegLat;
	float angleLong = 360.0f / (float)numSegLong;
	float x, y;
	x = 1.0f / (float)numSegLong;
	y = 1.0f / (float)numSegLat;

	Quaternion rotY = Quaternion::rotationAroundAxisVector(angleLong, worldRight);
	for (int i = 0; i <= numSegLat; i++)
	{
		glm::vec3 rad = glm::vec3(0, -1, 0) * radius;
		Quaternion rotX = Quaternion::rotationAroundAxisVector(i * angleLat, worldRight);
		
		rad = rotX * rad;
		for (int j = 0; j < numSegLong; j++)
		{
			vert v;
			v.normal = glm::normalize(rad);
			v.uv = glm::vec2(1);
			if (i == 0 || i == numSegLat)
			{
				v.uv.x = x * (numSegLong / 2);
				v.uv.y = 0;
				v.pos = rad;
				sphere.push_back(v);
				break;

			}
			else
			{
				v.pos = rad * Quaternion::rotationAroundAxisVector(j * angleLong, worldUp);
				v.normal = glm::normalize(v.pos);
				v.uv.x = j * x;
				v.uv.y = (i + 1) * y;
				sphere.push_back(v);
				if (i == 1)
				{
					if (j < (numSegLong - 1))
					{

						index.push_back(0);
						index.push_back(1 + j);
						index.push_back(j + 2);
					}
					else
					{
						index.push_back(0);
						index.push_back(1 + j);
						index.push_back(1);
					}

				}
				else
				{

					int currPos = 1 + ((i - 1) * (numSegLong)) + j;
					if (j < (numSegLong - 1))
					{

						index.push_back(currPos);
						index.push_back(currPos - (numSegLong));
						index.push_back(currPos - (numSegLong)+1);

						index.push_back(currPos);
						index.push_back(currPos + 1);
						index.push_back(currPos - (numSegLong)+1);
					}
					else
					{
						index.push_back(currPos);
						index.push_back(currPos - (numSegLong));
						index.push_back(currPos - (numSegLong)-j);

						index.push_back(currPos);
						index.push_back(currPos - j);
						index.push_back(currPos - (numSegLong)-j);
					}

					if (i == numSegLat - 1)
					{
						int lastPose = currPos - j + numSegLong;
						if (j < (numSegLong - 1))
						{

							index.push_back(lastPose);
							index.push_back(currPos);
							index.push_back(currPos + 1);
						}
						else
						{
							index.push_back(lastPose);
							index.push_back(currPos);
							index.push_back(currPos - j);
						}

					}
				}

			}

		}
	}
	ind = index;
	return sphere;
}
vert vertFromIndex(int i,int j,float radius,int numSegLat,int numSegLong)
{
	vert v;

	glm::vec3 rad = glm::vec3(0, -1, 0) * radius;
	float angleLat = 180.0f / (float)numSegLat;
	float angleLong = 360.0f / (float)numSegLong;
	float x, y;
	x = 1.0f / numSegLong;
	y = 1.0f / numSegLat;
	Quaternion rotY = Quaternion::rotationAroundAxisVector(i*angleLat, worldRight);
	Quaternion rotX = Quaternion::rotationAroundAxisVector(j*angleLong, worldUp);
	
	v.pos = rotY * rad;
	v.pos = rotX * v.pos;
	v.normal = glm::normalize(v.pos);
	if (i == 0 || i==numSegLat)
	{
		v.uv.x =(j*x)+(x/2.0f);
		v.uv.y = (y*i);
	}
	else
	{
		v.uv.x = j * x;
		v.uv.y = (i) * y;
	}

	return v;

}
std::vector<vert> generateSphereVertices(int numSegLat, int numSegLong, float radius)
{
	std::vector<vert> sphere;
	
	for (int i = 0; i <numSegLat; i++)
	{

		for (int j = 0; j < numSegLong; j++)
		{
			vert v,v1,v2;
			if (i == 0)
			{
				v=vertFromIndex(i,j,radius,numSegLat,numSegLong);
				v1=vertFromIndex(i+1,j,radius,numSegLat,numSegLong);
				v2=vertFromIndex(i+1,j+1,radius,numSegLat,numSegLong);
				sphere.push_back(v);
				sphere.push_back(v1);
				sphere.push_back(v2);
			}
			else if (i == numSegLat - 1)
			{
				
					v = vertFromIndex(i, j, radius, numSegLat, numSegLong);
					v1 = vertFromIndex(i, j + 1, radius, numSegLat, numSegLong);
					v2 = vertFromIndex(i + 1, j, radius, numSegLat, numSegLong);
					v2.pos = glm::vec3(0,1,0)*radius;
					sphere.push_back(v);
					sphere.push_back(v1);
					sphere.push_back(v2);
				
				
			}
			else 
			{
				v = vertFromIndex(i, j, radius, numSegLat, numSegLong);
				v1=vertFromIndex(i, j + 1, radius, numSegLat, numSegLong);
				v2=vertFromIndex(i + 1, j, radius, numSegLat, numSegLong);
				sphere.push_back(v);
				sphere.push_back(v1);
				sphere.push_back(v2);
			
				v = vertFromIndex(i, j + 1, radius, numSegLat, numSegLong);
				v1 = vertFromIndex(i + 1, j + 1, radius, numSegLat, numSegLong);
				v2 = vertFromIndex(i + 1, j, radius, numSegLat, numSegLong);
				sphere.push_back(v);
				sphere.push_back(v1);
				sphere.push_back(v2);
				
			}

			

		}
	}
	return sphere;
}



std::vector<vert> generateCubeVertices(int heightSeg, int widthSeg)
{
	std::vector<vert> cube;
	std::vector<vert> faceFront;

	float h = 1.0f/(float)heightSeg;
	float w = 1.0f/(float)widthSeg;

	float z = -0.5f;
	float x = -0.5f;
	float y = -0.5f;
	for (int i = 0; i < heightSeg; i++)
	{
		for (int j = 0; j < widthSeg; j++)
		{
			vert v1 = { {x+(1 + j) * (w),y+i * h,z},{0,0,-1},{(1+j)*w,i*h} };//(1,0)
			vert v2 = { {x+(1 + j) * (w),y+(i+1) * h,z},{0,0,-1},{(1 + j) * (w),(i + 1) * h} };//(1,1)
			vert v3 = { {x+(j) * (w),i* h+y,z},{0,0,-1},{(j) * (w),i * h} };//(0,0)
			vert v4 = { {x+(j) * (w),(i+1) * h+y,z},{0,0,-1},{(j) * (w),(i + 1) * h} };//(0,1)
			faceFront.push_back(v1);
			faceFront.push_back(v2);
			faceFront.push_back(v3);
			faceFront.push_back(v2);
			faceFront.push_back(v4);
			faceFront.push_back(v3);
		}
	}
	cube = faceFront;
	
	for (int i = 1; i <4; i++)
	{
		Quaternion rotate = Quaternion::rotationAroundAxisVector(i * 90, worldUp);

		for (int j = 0; j < faceFront.size(); j++)
		{
			vert v;
			v.pos= rotate*faceFront[j].pos;
			v.normal= rotate*faceFront[j].normal;
			v.uv= faceFront[j].uv;
			cube.push_back(v);
		}
	}
	
		Quaternion rotate = Quaternion::rotationAroundAxisVector(90, worldRight);

		for (int j = 0; j < faceFront.size(); j++)
		{
			vert v;
			v.pos = rotate * faceFront[j].pos;
			v.normal = rotate * faceFront[j].normal;
			v.uv = faceFront[j].uv;
			cube.push_back(v);
		}

		rotate = Quaternion::rotationAroundAxisVector(-90, worldRight);

		for (int j = 0; j < faceFront.size(); j++)
		{
			vert v;
			v.pos = rotate * faceFront[j].pos;
			v.normal = rotate * faceFront[j].normal;
			v.uv = faceFront[j].uv;
			cube.push_back(v);
		}
	
	return cube;
}

std::vector<vert> generatePlaneVertices(int lengthSeg, int widthSeg)
{
	std::vector<vert> faceFront;

	float h = 1.0f / (float)lengthSeg;
	float w = 1.0f / (float)widthSeg;

	float z = 0;
	float x = -0.5f;
	float y = -0.5f;
	for (int i = 0; i <lengthSeg; i++)
	{
		for (int j = 0; j < widthSeg; j++)
		{
			vert v1 = { {x + (1 + j) * (w),y + i * h,z},{0,0,-1},{(1 + j) * w,i * h} };//(1,0)
			vert v2 = { {x + (1 + j) * (w),y + (i + 1) * h,z},{0,0,-1},{(1 + j) * (w),(i + 1) * h} };//(1,1)
			vert v3 = { {x + (j) * (w),i * h + y,z},{0,0,-1},{(j) * (w),i * h} };//(0,0)
			vert v4 = { {x + (j) * (w),(i + 1) * h + y,z},{0,0,-1},{(j) * (w),(i + 1) * h} };//(0,1)
			faceFront.push_back(v1);
			faceFront.push_back(v2);
			faceFront.push_back(v3);
			faceFront.push_back(v2);
			faceFront.push_back(v4);
			faceFront.push_back(v3);
		}
	}

	Quaternion rotate = Quaternion::rotationAroundAxisVector(-90, worldRight);

	for (int j = 0; j < faceFront.size(); j++)
	{
		vert v;
		v.pos = rotate * faceFront[j].pos;
		v.normal = rotate * faceFront[j].normal;
		v.uv = faceFront[j].uv;
		faceFront[j] = v;
	}
	return faceFront;
}

inline lines::lines(glm::vec3 st, glm::vec3 end, glm::vec3 color) :debugShader("Shaders/DebugShader/default.vert", "Shaders/DebugShader/default.frag")
{
	this->color = color;
	//generate all buffers required for rendering and storing the mesh on the GPU
	vData.st = st;
	vData.end = end;
	//generate Vertex Buffer object
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vData), &vData, GL_STATIC_DRAW);

	//generate Vertex Attribute Object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//vertex coordinate
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(0));
	glEnableVertexAttribArray(0);

	//unBinding all the buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


}

inline void lines::renderMesh()
{
	glad_glPointSize(2);
	glad_glLineWidth(1);
	//if all the buffers were successfully generated, then render the mesh

	debugShader.useShaderProgram();
	debugShader.setUniformVec3("color", color);
	glBindVertexArray(VAO);

	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

inline void lines::clearMesh()
{
	if (VBO)
		glDeleteBuffers(1, &VBO);
	if (VAO)
		glDeleteVertexArrays(1, &VAO);

}
