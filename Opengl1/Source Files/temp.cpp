#include"temp.h"
#include<glad/glad.h>

void calTangentBiTangent(vert &v0,vert &v1,vert &v2)
{
	glm::vec3 t, b;
	glm::vec3 e1 = v0.pos-v1.pos;
	glm::vec3 e2 = v0.pos - v2.pos;

	glm::vec2 delv1 = v0.uv-v1.uv;
	glm::vec2 delv2 = v0.uv-v2.uv;
	/*******
	UV matrix 'U'
		|delv1.x  delv1.y|
		|delv2.x  delv2.y|

		inv(U):
		 1		
		--- x	|delv2.y  -delv1.y|
		|U|		|-delv2.x  delv1.x|
	edge matrix 'E':
		|e1.x e1.y e1.z|
		|e2.x e2.y e2.z|
	tangent and bitangent matrix  'T':
		|t.x t.y t.z|
		|b.x b.y b.z|

		to calculate T
			we know:  E=UxT
			premultiply by inverse of U
			we have:  inv(U)*E=T

			t= (1/|U|) * (delv2.y * e1 - delv1.y*e2)

	*******/

	//determinant of U
	float determU = (delv1.x * delv2.y - delv1.y * delv2.x);
	float const constant = 1.0f/determU;

	t = constant*(delv2.y * e1 - delv1.y * e2);
	b = constant * (-delv2.x * e1+delv1.x*e2);

	v0.tangent = t;
	v0.biTangent = b;
	v1.tangent = t;
	v1.biTangent = b;
	v2.tangent = t;
	v2.biTangent = b;

}

void calTangentBiTangent(std::vector<vert>& v)
{
	for (size_t i = 0; i < v.size(); i += 3)
	{
		calTangentBiTangent(v[i], v[i + 1], v[i+2]);
	}
}
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
				sphere.push_back(v2);
				sphere.push_back(v1);
				sphere.push_back(v);
				
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
	calTangentBiTangent(sphere);
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
			faceFront.push_back(v3);
			faceFront.push_back(v2);
			faceFront.push_back(v1);
			
			
			faceFront.push_back(v3);
			faceFront.push_back(v4);
			faceFront.push_back(v2);
			
		}
	}
	cube = faceFront;
	
	for (int i = 1; i <4; i++)
	{
		Quaternion rotate = Quaternion::rotationAroundAxisVector(i * 90.0f, worldUp);

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
		calTangentBiTangent(cube);
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
			faceFront.push_back(v3);
			faceFront.push_back(v2);
			faceFront.push_back(v1);
			
			
			faceFront.push_back(v3);
			faceFront.push_back(v4);
			faceFront.push_back(v2);
			
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
	calTangentBiTangent(faceFront);
	return faceFront;
}

lines::lines(glm::vec3 st, glm::vec3 end, glm::vec3 color) 
{
	std::string root = AssetManager::assetRootPath;
	debugShader = std::make_shared<Shader>(root+"Shaders/DebugShader/default.vert",root+"Shaders/DebugShader/default.frag");
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

void lines::renderMesh()
{
	glad_glPointSize(2);
	glad_glLineWidth(1);
	//if all the buffers were successfully generated, then render the mesh

	debugShader->useShaderProgram();
	debugShader->setUniformVec3("color", color);
	glBindVertexArray(VAO);

	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void lines::clearMesh()
{
	if (VBO)
		glDeleteBuffers(1, &VBO);
	if (VAO)
		glDeleteVertexArrays(1, &VAO);

}

