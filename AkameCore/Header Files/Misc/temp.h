#pragma once
#include"Core/Engine.h"
#include<vector>
class lines
{
	unsigned int VBO, VAO;
public:
	glm::vec3 color;
	std::shared_ptr<Shader> debugShader;
	struct line
	{
		glm::vec3 st;
		glm::vec3 end;
	}vData;
	lines(glm::vec3 st, glm::vec3 end, glm::vec3 color);

	void renderMesh();


	void clearMesh();

};



void calTangentBiTangent(vert& v0, vert& v1, vert& v2);
void calTangentBiTangent(std::vector<vert>& v);
std::vector<vert> generateSphereVertices(int numSegLat, int numSegLong, float radius, std::vector<unsigned int>& ind);
std::vector<vert> generateSphereVertices(int numSegLat, int numSegLong, float radius);
std::vector<vert> generateCubeVertices(int heightSeg=1, int widthSeg=1);
std::vector<vert> generatePlaneVertices(int lengthSeg=1, int widthSeg=1);

