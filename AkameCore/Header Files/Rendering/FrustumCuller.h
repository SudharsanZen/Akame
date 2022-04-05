#pragma once
#include"Math/Octree.h"
#include"Core/AkameCore.h"
class FrustumCuller
{
	Camera& m_cam;
	FRUSTUM m_view_frustum;
	std::vector<Entity> _render_list;
	std::shared_ptr<OctTree> m_octreee;
	AKAME_API void _get_culled_in_node(OctTreeNode* node);
public:
	AKAME_API FrustumCuller(Camera& m_cam);
	AKAME_API std::vector<Entity>& get_culled(std::shared_ptr<OctTree> octree);
};