#include"Rendering/FrustumCuller.h"



void FrustumCuller::_get_culled_in_node(OctTreeNode* node)
{
	if (node)
	{
		AABB octant_aabb;
		octant_aabb.max = node->halfExtent;
		octant_aabb.min = -node->halfExtent;
		octant_aabb.origin = node->origin;
		octant_aabb.half_extent = node->halfExtent;
		if (m_view_frustum.check_SPHERE_Intersection(octant_aabb))
		{
			for (auto& e_det : node->entityList)
			{
				AABB entity_obb;
				entity_obb.max = e_det.max;
				entity_obb.min = e_det.min;
				entity_obb.origin = e_det.position;

				if (m_view_frustum.check_SPHERE_Intersection(entity_obb))
					_render_list.push_back(e_det.eid);
			}

			for (auto& child : node->child)
			{
				_get_culled_in_node(child);
			}
		}
	}
}

FrustumCuller::FrustumCuller(Camera& m_cam) :m_cam(m_cam), m_view_frustum(m_cam)
{

}

std::vector<Entity>& FrustumCuller::get_culled(std::shared_ptr<OctTree> octree)
{
	m_view_frustum.DrawFrustum();
	m_view_frustum.CalcFrustumParameters();
	m_octreee = octree;
	_render_list.clear();
	OctTreeNode* head = octree->head;
	_get_culled_in_node(head);
	return _render_list;
}
