#include "Math/Octree.h"
#include"Core/Debug/Debug.h"
#include"Components/Rendering/Mesh.h"
#include"Core/Scene.h"
int OctTreeNode::maxLevel =5;
void OctTreeNode::SetMaxLevel(int level)
{
	maxLevel = level;

}

void OctTreeNode::DebugDrawBB()
{
	Debug::DrawBB(origin - halfExtent, origin + halfExtent, glm::vec3(0, 1, 0));
	for (int i = 0; i < 8; i++)
		if (child[i] != nullptr)
			child[i]->DebugDrawBB();
}

void OctTreeNode::DebugDrawSPHERE()
{
	Debug::DrawCircle(origin, worldUp, glm::length(halfExtent), glm::vec3(1, 0, 0));
	for (int i = 0; i < 8; i++)
		if (child[i])
			child[i]->DebugDrawSPHERE();
}

void OctTreeNode::DebugDrawBB(Camera& camera)
{
	Debug::DrawBB(origin - halfExtent, origin + halfExtent, glm::vec3(0, 1, 0));
	for (int i = 0; i < 8; i++)
		if (child[i] != nullptr)
			child[i]->DebugDrawBB(camera);
}

void OctTreeNode::setChildToNull()
{
	for (int i = 0; i < 8; i++)
		child[i] = nullptr;
}

OctTreeNode::OctTreeNode(Scene& m_scene) :m_scene(m_scene), child(8, nullptr)
{
	currLevel = 0;
	parent = nullptr;
	setChildToNull();
	origin = glm::vec3(0);
	halfExtent = glm::vec3(250);

}

OctTreeNode::OctTreeNode(Scene& m_scene, glm::vec3 origin, glm::vec3 halfExt, int currLevel, int maxLevel) :OctTreeNode(m_scene)
{
	this->origin = origin;
	halfExtent = halfExt;
	this->currLevel = currLevel;
	setChildToNull();
}

bool OctTreeNode::isPointInOctane(glm::vec3 point)
{
	glm::vec3 max = origin + halfExtent;
	glm::vec3 min = origin - halfExtent;
	return (point.x<max.x&& point.x>min.x &&
		point.z<max.z&& point.z>min.z &&
		point.y<max.y&& point.y>min.y);
}

bool OctTreeNode::isPointInOctane(glm::vec3 min, glm::vec3 max, glm::vec3 point)
{
	return (point.x<max.x&& point.x>min.x &&
		point.z<max.z&& point.z>min.z &&
		point.y<max.y&& point.y>min.y);
}

EntityOBBDetails OctTreeNode::getEntityDetails(Entity const& eid)
{
	Mesh& mesh = m_scene.GetComponent<Mesh>(eid);
	Transform& trans = m_scene.GetComponent<Transform>(eid);
	EntityOBBDetails details;
	details.min = mesh.GetMin();
	details.max = mesh.GetMax();
	details.transform = trans.transformMatrix();
	details.position = trans.GetGlobalPosition();
	details.eid = eid;
	std::vector<glm::vec3> verts(8, glm::vec3(0));
	verts[1] = details.transform * glm::vec4(details.max, 1);
	verts[2] = details.transform * glm::vec4(details.max.x, details.min.y, details.max.z, 1);
	verts[3] = details.transform * glm::vec4(details.min.x, details.min.y, details.max.z, 1);
	verts[4] = details.transform * glm::vec4(details.min.x, details.max.y, details.max.z, 1);
	verts[5] = details.transform * glm::vec4(details.min, 1);
	verts[6] = details.transform * glm::vec4(details.min.x, details.max.y, details.min.z, 1);
	verts[7] = details.transform * glm::vec4(details.max.x, details.max.y, details.min.z, 1);
	verts[0] = details.transform * glm::vec4(details.max.x, details.min.y, details.min.z, 1);

	details.OBB_verts = verts;
	return details;
}

bool OctTreeNode::isOBBInOctant(EntityOBBDetails& details)
{
	for (int i = 0; i < 8; i++)
		if (!isPointInOctane(details.OBB_verts[i]))
			return false;
	return true;
}

bool OctTreeNode::isOBBInOctant(EntityOBBDetails& details, glm::vec3 min, glm::vec3 max)
{
	for (int i = 0; i < 8; i++)
		if (!isPointInOctane(min, max, details.OBB_verts[i]))
			return false;
	return true;
}

bool OctTreeNode::insert(const Entity& eid)
{
	EntityOBBDetails detials = getEntityDetails(eid);
	return _insert(detials);

}

AABB OctTreeNode::GetOctaneAABB(Octant octant)
{
	glm::vec3 originOff = { 0,0,0 };

	switch (octant)
	{
	case Octant::FTR:
		originOff = glm::vec3(halfExtent);
		break;
	case Octant::FTL:
		originOff = glm::vec3(-halfExtent.x, halfExtent.y, halfExtent.z);
		break;
	case Octant::FBR:
		originOff = glm::vec3(halfExtent.x, -halfExtent.y, halfExtent.z);
		break;
	case Octant::FBL:
		originOff = glm::vec3(-halfExtent.x, -halfExtent.y, halfExtent.z);
		break;
	case Octant::BTR:
		originOff = glm::vec3(halfExtent.x, halfExtent.y, -halfExtent.z);
		break;
	case Octant::BTL:
		originOff = glm::vec3(-halfExtent.x, halfExtent.y, -halfExtent.z);
		break;
	case Octant::BBR:
		originOff = glm::vec3(halfExtent.x, -halfExtent.y, -halfExtent.z);
		break;
	case Octant::BBL:
		originOff = glm::vec3(-halfExtent.x, -halfExtent.y, -halfExtent.z);

	}
	glm::vec3 n_origin = origin + (originOff / 2.0f);
	glm::vec3 n_min = n_origin - (halfExtent / 2.0f);
	glm::vec3 n_max = n_origin + halfExtent / 2.0f;

	return { n_origin,halfExtent / 2.0f,n_min,n_max };
}

bool OctTreeNode::isOBBInOctant(EntityOBBDetails& details, AABB octant)
{

	return isOBBInOctant(details, octant.min, octant.max);
}

bool OctTreeNode::_insert(EntityOBBDetails& details)
{

	for (int i = 0; i < 8 && currLevel != maxLevel; i++)
	{
		AABB aabb = GetOctaneAABB(static_cast<Octant>(i));
		if (isOBBInOctant(details, aabb.min, aabb.max))
		{
			if (child[i] == nullptr)
				child[i] = new OctTreeNode(m_scene, aabb.origin, aabb.half_extent, currLevel + 1, maxLevel);

			return child[i]->_insert(details);
		}
	}
	if (isOBBInOctant(details))
	{
		entityList.push_back(details);
		return true;
	}
	return false;
}

OctTreeNode::~OctTreeNode()
{
	_free_tree();
}

void OctTreeNode::_free_tree()
{
	for (int i = 0; i < 8; i++)
	{
		if (child[i] != nullptr)
		{
			child[i]->_free_tree();
			delete child[i];
			child[i] = nullptr;
		}
	}
	parent = nullptr;
	currLevel = 0;
}

OctTree::OctTree(Scene& m_scene) :m_scene(m_scene)
{
	head = nullptr;
}

Octant OctTree::GetOpposite(Octant curr)
{
	switch (curr)
	{
	case Octant::FTR:
		return Octant::BBL;
		break;
	case Octant::FTL:
		return Octant::BBR;
		break;
	case Octant::FBR:
		return Octant::BTL;
		break;
	case Octant::FBL:
		return Octant::BTR;
		break;
	case Octant::BTR:
		return Octant::FBL;
		break;
	case Octant::BTL:
		return Octant::FBR;
		break;
	case Octant::BBR:
		return Octant::FTL;
		break;
	case Octant::BBL:
		return Octant::FTR;
		break;
	default:
		AK_ASSERT("INVALID ENUM CLASS");
		break;
	}
}

bool OctTree::insert(const Entity& eid)
{
	if (head == nullptr)
	{
		head = new OctTreeNode(m_scene);
	}
	if (head->insert(eid))
		return true;
	else
	{
		glm::vec3 halfExtent = head->halfExtent;
		glm::vec3 origin = head->origin;
		AABB ftr;
		ftr.half_extent = halfExtent * 2.0f;
		ftr.origin = origin + ftr.half_extent;
		ftr.max = ftr.origin + ftr.half_extent;
		ftr.min = ftr.origin - ftr.half_extent;

		AABB ftl;
		ftl.half_extent = halfExtent * 2.0f;
		ftl.origin = origin + glm::vec3(-halfExtent.x, halfExtent.y, halfExtent.z);
		ftl.max = ftl.origin + ftl.half_extent;
		ftl.min = ftl.origin - ftl.half_extent;

		AABB fbr;
		fbr.half_extent = halfExtent * 2.0f;
		fbr.origin = origin + glm::vec3(halfExtent.x, -halfExtent.y, halfExtent.z);
		fbr.max = fbr.origin + fbr.half_extent;
		fbr.min = fbr.origin - fbr.half_extent;

		AABB fbl;
		fbl.half_extent = halfExtent * 2.0f;
		fbl.origin = origin + glm::vec3(-halfExtent.x, -halfExtent.y, halfExtent.z);
		fbl.max = fbl.origin + fbl.half_extent;
		fbl.min = fbl.origin - fbl.half_extent;

		AABB btr;
		btr.half_extent = halfExtent * 2.0f;
		btr.origin = origin + glm::vec3(halfExtent.x, halfExtent.y, -halfExtent.z);
		btr.max = btr.origin + btr.half_extent;
		btr.min = btr.origin - btr.half_extent;

		AABB btl;
		btl.half_extent = halfExtent * 2.0f;
		btl.origin = origin + glm::vec3(-halfExtent.x, halfExtent.y, -halfExtent.z);
		btl.max = btl.origin + btl.half_extent;
		btl.min = btl.origin - btl.half_extent;

		AABB bbr;
		bbr.half_extent = halfExtent * 2.0f;
		bbr.origin = origin + glm::vec3(halfExtent.x, -halfExtent.y, -halfExtent.z);
		bbr.max = bbr.origin + bbr.half_extent;
		bbr.min = bbr.origin - bbr.half_extent;

		AABB bbl;
		bbl.half_extent = halfExtent * 2.0f;
		bbl.origin = origin + glm::vec3(-halfExtent.x, -halfExtent.y, -halfExtent.z);
		bbl.max = bbl.origin + bbl.half_extent;
		bbl.min = bbl.origin - bbl.half_extent;

		std::vector<AABB> bblist = { ftr,ftl,fbr,fbl,btr,btl,bbr,bbl };
		EntityOBBDetails details = head->getEntityDetails(eid);
		int currLevel = head->currLevel;
		int maxLevel = head->maxLevel;
		OctTreeNode* parent = nullptr;
		for (int i = 0; i < 8; i++)
		{
			if (OctTreeNode::isOBBInOctant(details, bblist[i]))
			{
				parent = new OctTreeNode(m_scene, bblist[i].origin, bblist[i].half_extent, currLevel - 1, maxLevel);
				parent->child[static_cast<int>(GetOpposite(static_cast<Octant>(i)))] = head;
			}
		}

		if (parent == nullptr)
		{
			float dist = -1.0f;
			Octant curr;
			AABB currBB;
			for (int i = 0; i < 8; i++)
			{
				glm::vec3 meshOrigin = (details.transform * glm::vec4((details.max + details.min) / 2.0f, 1));
				float _dist = glm::length(bblist[i].origin - meshOrigin);
				if (dist < -0.1f || dist>_dist)
				{
					curr = static_cast<Octant>(i);
					dist = _dist;
					currBB = bblist[i];
				}
			}
			parent = new OctTreeNode(m_scene, currBB.origin, currBB.half_extent, currLevel - 1, maxLevel);
			parent->child[static_cast<int>(GetOpposite(curr))] = head;
		}

		assert(parent != nullptr && "something went wrong, parent pointer is not supposed to be null here");
		head = parent;
		insert(eid);
	}
	return true;
}

void OctTree::DrawTreeBox()
{
	if (head != nullptr)
		head->DebugDrawBB();
}

void OctTree::DrawTreeSphere()
{
	if (head)
		head->DebugDrawSPHERE();
}

OctTree::~OctTree()
{
	_free();
}

void OctTree::_free()
{
	//if (head != nullptr)
	//std::cout << head->maxLevel << "\n";
	if (head != nullptr)
	{
		delete head;
		head = nullptr;
	}
}
