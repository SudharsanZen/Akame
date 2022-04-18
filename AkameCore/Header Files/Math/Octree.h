#pragma once
#include<vector>
#include"Math/Shapes.h"
#include"Core/AkameCore.h"
class Scene;
/*
*				octants
*         *-----------------*
*        /  BTL   /  BTR   /|
*	    /-----------------/	|
*      /        /        /|	|
*     *-----------------* |/|
*	  |	FTL	   |  FTR   | |	|  <-BBR
*	  |		   |        |/|	*
* 	  |--------|--------| |/
* 	  |	FBL	   |  FBR   | /
*	  |		   |        |/
*     *-----------------*
*/

enum class Octant :int
{
	FTR,	//front top right
	FTL,	//front top left
	FBR,	//front back right
	FBL,	//front back left
	BTR,	//back top right
	BTL,	//back top left
	BBR,	//back bottom right
	BBL,	//back bottom left
};
class OctTreeNode
{
private:
	friend class OctTree;
	friend class FrustumCuller;
	friend class PSSMCuller;
	struct _vert
	{
		glm::vec3 point;
		glm::vec3 color;
	};
	static int maxLevel;
	static int minLevel;
	int currLevel;
	OctTreeNode* parent;
	std::vector<OctTreeNode*> child;
	glm::vec3 origin;
	glm::vec3 halfExtent;

	Scene& m_scene;
	std::set<EntityOBBDetails> entityList;
public:
	AABB aabb;
	AKAME_API static void SetMaxLevel(int level);
	AKAME_API static void SetMinLevel(int level);
	AKAME_API void DebugDrawBB();
	AKAME_API void DebugDrawSPHERE();
	AKAME_API void DebugDrawBB(Camera& camera);
	AKAME_API void setChildToNull();
	AKAME_API OctTreeNode(Scene& m_scene);
	AKAME_API OctTreeNode(Scene& m_scene, glm::vec3 origin, glm::vec3 halfExt, int currLevel, int maxLevel);
	AKAME_API bool isPointInOctane(glm::vec3 point);
	AKAME_API static bool isPointInOctane(glm::vec3 min, glm::vec3 max, glm::vec3 point);
	AKAME_API EntityOBBDetails getEntityDetails(const Entity& eid);
	AKAME_API bool isOBBInOctant(const EntityOBBDetails& details);
	AKAME_API static bool isOBBInOctant(EntityOBBDetails& details, glm::vec3 min, glm::vec3 max);
	AKAME_API bool insert(const Entity& eid);
	AKAME_API AABB GetOctaneAABB(Octant octant);
	AKAME_API static bool isOBBInOctant(EntityOBBDetails& details, AABB octant);
	AKAME_API bool _insert(EntityOBBDetails& details);
	AKAME_API ~OctTreeNode();
	AKAME_API OctTreeNode* find(const EntityOBBDetails &e_det);
	AKAME_API bool remove_entity(const EntityOBBDetails &e_det);

	AKAME_API void _free_tree();

};

class OctTree
{
	Scene& m_scene;
	size_t m_size;
public:

	OctTreeNode* head;
	AKAME_API size_t size();
	AKAME_API OctTree(Scene& m_scene);
	AKAME_API Octant GetOpposite(Octant curr);
	AKAME_API bool insert(const Entity& eid);
	AKAME_API OctTreeNode* find(const Entity& eid);
	AKAME_API void DrawTreeBox();
	AKAME_API void DrawTreeSphere();
	AKAME_API bool remove_entity(const Entity&eid);
	AKAME_API ~OctTree();
	AKAME_API void _free();
};