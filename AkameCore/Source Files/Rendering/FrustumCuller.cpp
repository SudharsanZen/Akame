#include"Rendering/FrustumCuller.h"
#include"Rendering/System/PSSMFBO.h"


void FrustumCuller::_get_culled_in_node(OctTreeNode* node)
{
	if (node)
	{
		AABB octant_aabb;
		octant_aabb.set_half_extents_origin(node->origin,node->halfExtent);
		if (m_view_frustum.check_AABB_Intersection(octant_aabb))
		{
			for (auto& e_det : node->entityList)
			{
				if (m_view_frustum.check_AABB_Intersection(e_det.aabb))
					_render_list.push_back(&(e_det.eid));
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

std::vector<const Entity*>& FrustumCuller::get_culled(std::shared_ptr<OctTree> octree)
{

	m_view_frustum.CalcFrustumParameters();
	//m_view_frustum.DrawFrustum();
	m_octreee = octree;
	_render_list.clear();
	_render_list.reserve(octree->size());
	OctTreeNode* head = octree->head;
	_get_culled_in_node(head);
	return _render_list;
}

void PSSMCuller::_get_culled_in_node(OctTreeNode* node)
{
	if (node == nullptr)
		return;
	bool intersects = false;
	for (int i = 0; i < bound_list.size(); i++)
	{
		if (node->aabb.check_aabb_intersection(bound_list[i]))
		{
			intersects = true;
			break;
		}
	}
	if (intersects)
	{
		for (auto& ent : node->entityList)
			for (int j = 0; j < bound_list.size(); j++)
				if (ent.aabb.check_aabb_intersection(bound_list[j]))
				{
					_render_list.push_back(&(ent.eid));
					break;
				}

		for (auto& child : node->child)
		{
			if(child)
				_get_culled_in_node(child);
		}
		

	}
	
}

PSSMCuller::PSSMCuller(Camera cam, glm::vec3  l, int numOfFrustums, float lambda, float shadowDist) :m_cam(cam)
{
	bound_list = Calculate_PSSM_AABB_Bounds(cam,l,numOfFrustums,lambda,shadowDist);
}

std::vector<const Entity*>& PSSMCuller::get_culled(std::shared_ptr<OctTree> octree)
{
	//returns a list of pointers to entities within the shadow map bounds
	_render_list.clear();
	_render_list.reserve(octree->size());
	_get_culled_in_node(octree->head);

	return _render_list;
}
