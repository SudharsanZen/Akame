#include"temp.h"
GLfloat tvertices[16] = {
	-0.5,-0.5,0,		0,0,
	0.5,-0.5,0,			1,0,
	0,0.5,0 ,			0.5,1
};
GLuint tIndex[3] = { 0,1,2 };

float plane[32] = {
	// positions          // colors           // texture coords
	 0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,   // top right
	 0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f    // top left 
};

float planevert[20] = {
	// positions            // texture coords
	 0.5f,  0.5f, 0.0f,      1.0f, 1.0f,   // top right
	 0.5f, -0.5f, 0.0f,     1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,     0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,      0.0f, 1.0f    // top left 
};

GLuint planeIndex[6] = { 0,1,2,2,3,0 };

float boxvertices[180] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
float boxNormVertices[288] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};




std::vector<vert> sphere(int numSegLat, int numSegLong, float radius, std::vector<GLuint>& ind)
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
std::vector<vert> sphere(int numSegLat, int numSegLong, float radius)
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
GLfloat* serializeVec(std::vector<vert> v)
{
	size_t n = v.size() * 8;
	GLfloat* vPtr =new GLfloat[n];


	for (size_t i = 0; i < n; i++)
	{
		size_t pos = i / 8;
		switch (i%8)
		{
			case 0:
				vPtr[i] = v[pos].pos.x;
				break;

			case 1:
				vPtr[i] = v[pos].pos.y;
				break;

			case 2:
				vPtr[i] = v[pos].pos.z;
				break;

			case 3:
				vPtr[i] = v[pos].normal.x;
				break;

			case 4:
				vPtr[i] = v[pos].normal.y;
				break;

			case 5:
				vPtr[i] = v[pos].normal.z;
				break;

			case 6:
				vPtr[i] = v[pos].uv.x;
				break;

			case 7:
				vPtr[i] = v[pos].uv.y;
				break;
		}
	}
	return vPtr;

}