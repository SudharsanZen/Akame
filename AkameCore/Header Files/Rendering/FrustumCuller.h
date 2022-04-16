#pragma once
#include"Math/Octree.h"
#include"Core/AkameCore.h"
class FrustumCuller
{
	Camera& m_cam;
	VIEW_FRUSTUM m_view_frustum;
	std::vector<const Entity*> _render_list;
	std::shared_ptr<OctTree> m_octreee;
	AKAME_API void _get_culled_in_node(OctTreeNode* node);
public:
	AKAME_API FrustumCuller(Camera& m_cam);
	AKAME_API std::vector<const Entity*>& get_culled(std::shared_ptr<OctTree> octree);
};

class PSSMCuller
{
	Camera m_cam;
	std::vector<AABB> bound_list;
	std::vector<const Entity*> _render_list;
	std::shared_ptr<OctTree> m_octreee;
	AKAME_API void _get_culled_in_node(OctTreeNode* node);
public:
	AKAME_API PSSMCuller(Camera cam, glm::vec3  l, int numOfFrustums, float lambda, float shadowDistm);
	AKAME_API std::vector<const Entity*>& get_culled(std::shared_ptr<OctTree> octree);
};