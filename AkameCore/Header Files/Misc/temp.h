#pragma once
#include"Core/Engine.h"
#include<vector>
#include"Core/AkameCore.h"
class AKAME_API lines
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



template<typename v>
void calTangentBiTangent(v& v0, v& v1, v& v2)
{
	glm::vec3 t, b;
	glm::vec3 e1 = v0.pos - v1.pos;
	glm::vec3 e2 = v0.pos - v2.pos;

	glm::vec2 delv1 = v0.uv - v1.uv;
	glm::vec2 delv2 = v0.uv - v2.uv;
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
	float const constant = 1.0f / determU;

	t = constant * (delv2.y * e1 - delv1.y * e2);
	b = constant * (-delv2.x * e1 + delv1.x * e2);

	v0.tangent = t;
	v0.biTangent = b;
	v1.tangent = t;
	v1.biTangent = b;
	v2.tangent = t;
	v2.biTangent = b;

}
void AKAME_API calTangentBiTangent(std::vector<vert>& v);
std::vector<vert> AKAME_API generateSphereVertices(int numSegLat, int numSegLong, float radius, std::vector<unsigned int>& ind);
std::vector<vert> AKAME_API generateSphereVertices(int numSegLat, int numSegLong, float radius);
std::vector<vert> AKAME_API generateCubeVertices(int heightSeg=1, int widthSeg=1);
std::vector<vert> AKAME_API generatePlaneVertices(int lengthSeg=1, int widthSeg=1);
				  
