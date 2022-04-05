#include<iostream>

#define AK_PRIVATE_GETTER_SETTER
#include<fstream>
#include<string>
#include<algorithm>
#include<vector>
#include<stdlib.h>
#include<Core/Engine.h>
#include<Rendering/DeferredRendererFragmentBuffer.h>
#include<Core/Scene.h>
#include<Core/Debug/Debug.h>
#include<Core/Reflection/ReflectionUIHandler.h>
#include<Core/Reflection/ReflectionMeta.h>
#define sizeX 10.0f
#define sizeY 10.0f
#include"Rendering/System/RenderingSystem.h"
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
enum class Octant:int 
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

struct AABB
{
	glm::vec3 origin;
	glm::vec3 half_extent;
	glm::vec3 min;
	glm::vec3 max;
};
class strafeRandom:public Behaviour
{

private:
	int* level;
	AK_SHOW_IN_INSPECTOR
	(
		AK_ID(*level)
		AK_ID(baseSpeed)
		AK_ID(speedMultiplier)
	)
	glm::vec3 moveDir;
	static float baseSpeed;
	static float speedMultiplier;
	float speed;
	float time;
public:
	void OnStart()override
	{
		time = 0.0f;
	}
	strafeRandom(float pose,int *treeDepth)
	{
		level = treeDepth;
		speed =  rand() % 20;
		moveDir = worldForward*Quaternion(pose,pose*30,pose+60);
		moveDir = glm::normalize(moveDir);
	}
	void Update(float delta)override
	{
		time += delta;
		//Transform& trans = GetComponent<Transform>();
		//trans.SetGlobalPosition(trans.GetGlobalPosition()+moveDir*(speed+baseSpeed)*speedMultiplier*sin(time*1)*delta);
	}
};
float strafeRandom::baseSpeed = 10;
float strafeRandom::speedMultiplier = 1;
struct EntityOBBDetails
{
	Entity eid;
	std::vector<glm::vec3> OBB_verts;
	glm::vec3 min, max;
	glm::vec3 position;
	glm::mat4 transform;

};

class OctTreeNode
{
private:
	friend class OctTree;
	friend class FrustumCuller;
	struct _vert
	{
		glm::vec3 point;
		glm::vec3 color;
	};
	static int maxLevel;
	int currLevel;
	OctTreeNode* parent;
	std::vector<OctTreeNode*> child;
	glm::vec3 origin;
	glm::vec3 halfExtent;
	Scene& m_scene;
	std::vector<EntityOBBDetails> entityList;
public:
	static void SetMaxLevel(int level)
	{
		maxLevel = level;

	}
	void DebugDrawBB()
	{
		Debug::DrawBB(origin-halfExtent,origin+halfExtent,glm::vec3(0,1,0));
		for (int i = 0; i < 8; i++)
			if (child[i] != nullptr)
				child[i]->DebugDrawBB();
	}
	void DebugDrawSPHERE()
	{
		Debug::DrawCircle(origin,worldUp,glm::length(halfExtent),glm::vec3(1,0,0));
		for (int i = 0; i < 8; i++)
			if (child[i])
				child[i]->DebugDrawSPHERE();
	}
	void DebugDrawBB(Camera &camera)
	{
		Debug::DrawBB(origin - halfExtent, origin + halfExtent, glm::vec3(0, 1, 0));
		for (int i = 0; i < 8; i++)
			if (child[i] != nullptr)
				child[i]->DebugDrawBB(camera);
	}
	void setChildToNull()
	{
		for (int i = 0; i < 8; i++)
			child[i] = nullptr;
	}
	OctTreeNode(Scene& m_scene):m_scene(m_scene),child(8,nullptr)
	{
		currLevel = 0;
		parent = nullptr;
		setChildToNull();
		origin = glm::vec3(0);
		halfExtent = glm::vec3(250);

	}
	OctTreeNode(Scene& m_scene,glm::vec3 origin,glm::vec3 halfExt,int currLevel,int maxLevel):OctTreeNode(m_scene)
	{
		this->origin = origin;
		halfExtent = halfExt;
		this->currLevel = currLevel;
		setChildToNull();
	}
	bool isPointInOctane(glm::vec3 point)
	{
		glm::vec3 max = origin + halfExtent;
		glm::vec3 min = origin - halfExtent;
		return (point.x<max.x&& point.x>min.x &&
			point.z<max.z&& point.z>min.z &&
			point.y<max.y&& point.y>min.y);
	}
	static bool isPointInOctane(glm::vec3 min,glm::vec3 max,glm::vec3 point)
	{
		return (point.x<max.x&& point.x>min.x &&
			point.z<max.z&& point.z>min.z &&
			point.y<max.y&& point.y>min.y);
	}
	EntityOBBDetails getEntityDetails(Entity const &eid)
	{
		Mesh& mesh = m_scene.GetComponent<Mesh>(eid);
		Transform& trans = m_scene.GetComponent<Transform>(eid);
		EntityOBBDetails details;
		details.min = mesh.GetMin();
		details.max = mesh.GetMax();
		details.transform = trans.transformMatrix();
		details.position = trans.GetGlobalPosition();
		details.eid = eid;
		std::vector<glm::vec3> verts(8,glm::vec3(0));
		verts[1] = details.transform * glm::vec4(details.max, 1);
		verts[2] = details.transform * glm::vec4(details.max.x, details.min.y, details.max.z, 1);
		verts[3] = details.transform * glm::vec4(details.min.x, details.min.y, details.max.z, 1);
		verts[4] = details.transform * glm::vec4(details.min.x, details.max.y, details.max.z, 1);
		verts[5] = details.transform * glm::vec4(details.min, 1);
		verts[6] = details.transform * glm::vec4(details.min.x, details.max.y, details.min.z, 1);
		verts[7] = details.transform * glm::vec4(details.max.x, details.max.y, details.min.z, 1);
		verts[0] = details.transform * glm::vec4(details.max.x, details.min.y, details.min.z, 1);
		
		details.OBB_verts=verts;
		return details;
	}
	bool isOBBInOctant(EntityOBBDetails &details)
	{
		for (int i = 0; i < 8; i++)
			if (!isPointInOctane(details.OBB_verts[i]))
				return false;
		return true;
	}
	static bool isOBBInOctant(EntityOBBDetails& details,glm::vec3 min,glm::vec3 max)
	{
		for (int i = 0; i < 8; i++)
			if (!isPointInOctane(min,max, details.OBB_verts[i]))
				return false;
		return true;
	}
	bool insert(const Entity& eid)
	{
		EntityOBBDetails detials=getEntityDetails(eid);
		return _insert(detials);

	}
	AABB GetOctaneAABB(Octant octant)
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

		return {n_origin,halfExtent/2.0f,n_min,n_max};
	}
	static bool isOBBInOctant(EntityOBBDetails& details,AABB octant)
	{
		
		return isOBBInOctant(details,octant.min,octant.max);
	}
	bool _insert(EntityOBBDetails& details)
	{
		
		for (int i = 0; i < 8 && currLevel!=maxLevel; i++)
		{
			AABB aabb=GetOctaneAABB(static_cast<Octant>(i));
			if (isOBBInOctant(details,aabb.min,aabb.max))
			{
				if (child[i]==nullptr)
					child[i] = new OctTreeNode(m_scene,aabb.origin,aabb.half_extent,currLevel+1,maxLevel);
				
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
	~OctTreeNode()
	{
		_free_tree();
	}

	void _free_tree()
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

};
int OctTreeNode::maxLevel = 7;
class OctTree
{
	Scene& m_scene;
public:
	
	OctTreeNode* head;
	OctTree(Scene &m_scene):m_scene(m_scene)
	{
		head = nullptr;
	}
	Octant GetOpposite(Octant curr)
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
		
		}
	}
	bool insert(const Entity&  eid)
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

			std::vector<AABB> bblist = {ftr,ftl,fbr,fbl,btr,btl,bbr,bbl};
			EntityOBBDetails details = head->getEntityDetails(eid);
			int currLevel = head->currLevel;
			int maxLevel = head->maxLevel;
			OctTreeNode* parent=nullptr;
			for (int i = 0; i < 8; i++)
			{
				if (OctTreeNode::isOBBInOctant(details, bblist[i]))
				{
					parent = new OctTreeNode(m_scene, bblist[i].origin, bblist[i].half_extent, currLevel - 1, maxLevel);
					parent->child[static_cast<int>(GetOpposite(static_cast<Octant>(i)))] = head;
				}
			}
			
			if(parent==nullptr)
			{
				float dist =-1.0f;
				Octant curr;
				AABB currBB;
				for (int i = 0; i < 8; i++)
				{
					glm::vec3 meshOrigin = (details.transform*glm::vec4((details.max+details.min)/2.0f,1));
					float _dist=glm::length(bblist[i].origin-meshOrigin);
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
			
			assert(parent!=nullptr && "something went wrong, parent pointer is not supposed to be null here");
			head = parent;
			insert(eid);
		}
		return true;
	}
	void DrawTreeBox()
	{
		if(head!=nullptr)
			head->DebugDrawBB();
	}
	void DrawTreeSphere()
	{
		if (head)
			head->DebugDrawSPHERE();
	}
	
	~OctTree()
	{
		_free();
	}
	void _free()
	{
		//if (head != nullptr)
			//std::cout << head->maxLevel << "\n";
		if (head != nullptr)
		{
			delete head;
			head = nullptr;
		}
	}
};

struct PLANE
{
	glm::vec3 normal;
	glm::vec3 pop;//point on plane

	float getDistanceOfPointFromPlane(glm::vec3 point)
	{
		return glm::dot(normal,point-pop);
	}
	float getDistanceOfPointFromPlane(glm::vec3 point, float displace)
	{
		return glm::dot(normal, point - (pop+normal*displace));
	}

};
struct FRUSTUM
{
	PLANE m_far_plane;
	PLANE m_near_plane;
	PLANE m_right_plane;
	PLANE m_left_plane;
	PLANE m_top_plane;
	PLANE m_bottom_plane;

	float m_fov;
	float m_near;
	float m_far;
	float m_aspect_ratio;
	glm::vec3 m_camera_pose;
	std::vector<glm::vec3> m_near_vert;
	std::vector<glm::vec3> m_far_vert;
	Camera& m_camera;
	FRUSTUM(Camera& m_cam):m_near_vert(4),m_far_vert(4),m_camera(m_cam)
	{
		CalcFrustumParameters();
	}

	void CalcFrustumParameters()
	{
		m_fov = m_camera.getFov();
		
		m_camera_pose = m_camera.getCameraPosition();
		m_aspect_ratio = m_camera.getAspectRatio();
		m_near=m_camera.getNear();
		m_far = m_camera.getFar();
		
		float near_height = glm::tan(m_fov/2.0f)*m_near;
		float near_width  = m_aspect_ratio*near_height;
		float far_height  = glm::tan(m_fov)*m_far;
		float far_width  = m_aspect_ratio*far_height;

		glm::vec3 right = m_camera.transform.right();
		glm::vec3 up = m_camera.transform.up();
		glm::vec3 forward = m_camera.transform.forward();

		m_near_plane.normal = -forward;
		m_far_plane.normal = forward;
		
		m_near_plane.pop = m_camera_pose+forward*m_near;
		m_far_plane.pop = m_camera_pose+forward*m_far;
		//the points are calculated in clockwise order starting from the point in positive quadrant
		m_near_vert[0] = forward*m_near+m_camera_pose+right*near_width+up*near_height;
		m_near_vert[1] = forward*m_near+m_camera_pose+right*near_width-up*near_height;
		m_near_vert[2] = forward*m_near+m_camera_pose-right*near_width-up*near_height;
		m_near_vert[3] = forward*m_near+m_camera_pose-right*near_width+up*near_height;
		//the points are calculated in clockwise order starting from the point in positive quadrant
		m_far_vert[0] = forward*m_far+m_camera_pose + right * far_width + up * far_height;
		m_far_vert[1] = forward*m_far+m_camera_pose + right * far_width - up * far_height;
		m_far_vert[2] = forward*m_far+m_camera_pose - right * far_width - up * far_height;
		m_far_vert[3] = forward*m_far+m_camera_pose - right * far_width + up * far_height;

		//calculating right plane
		glm::vec3 a = glm::normalize(m_far_vert[1] - m_near_vert[0]);
		glm::vec3 b = glm::normalize(m_far_vert[0] - m_near_vert[0]);
		m_right_plane.normal = glm::normalize(glm::cross(a, b));
		m_right_plane.pop = m_far_vert[1];
		//calculating left plane
		a = glm::normalize(m_far_vert[3] - m_near_vert[3]);
		b = glm::normalize(m_far_vert[2] - m_near_vert[3]);
		m_left_plane.normal = glm::normalize(glm::cross(a, b));
		m_left_plane.pop = m_far_vert[3];
		//calculating top plane
		a = glm::normalize(m_far_vert[0] - m_near_vert[0]);
		b = glm::normalize(m_far_vert[3] - m_near_vert[0]);
		m_top_plane.normal = glm::normalize(glm::cross(a, b));
		m_top_plane.pop = m_far_vert[0];
		//calculating bottom plane
		a = glm::normalize(m_far_vert[2] - m_near_vert[1]);
		b = glm::normalize(m_far_vert[1] - m_near_vert[1]);
		m_bottom_plane.normal = glm::normalize(glm::cross(a, b));
		m_bottom_plane.pop = m_far_vert[1];


	}

	/*
	bool check_AABB_Intersection(AABB& aabb)
	{
		//near,far,right,left,top,bottom
		std::vector<glm::vec3> aabbVert; 
		aabbVert.insert(aabbVert.end(),m_near_vert.begin(),m_near_vert.end());
		aabbVert.insert(aabbVert.end(),m_far_vert.begin(),m_far_vert.end());
		
	}
	*/

	bool check_SPHERE_Intersection(AABB& aabb)
	{
		glm::vec3 origin = (aabb.max+aabb.min)/2.0f+aabb.origin;
		float radius = glm::length((aabb.max - aabb.min)) / 2.0f;
		Debug::DrawCircle(origin,worldLeft,0.2f,glm::vec3(0,1,0));
		return	(m_bottom_plane.getDistanceOfPointFromPlane(origin, radius) < 0)&&
				(m_top_plane.getDistanceOfPointFromPlane(origin, radius) < 0)&&
				(m_left_plane.getDistanceOfPointFromPlane(origin, radius) < 0)&&
				(m_right_plane.getDistanceOfPointFromPlane(origin, radius) < 0)&&
				(m_far_plane.getDistanceOfPointFromPlane(origin, radius) < 0)&&
				(m_near_plane.getDistanceOfPointFromPlane(origin, radius) < 0);
	}

	void DrawFrustum()
	{
		Debug::DrawRay(m_near_plane.pop,m_near_plane.normal,1,glm::vec3(0,1,0));
		Debug::DrawRay(m_far_plane.pop,m_far_plane.normal,1, glm::vec3(0, 1, 0));
		Debug::DrawRay(m_right_plane.pop,m_right_plane.normal,1, glm::vec3(0, 1, 0));
		Debug::DrawRay(m_left_plane.pop,m_left_plane.normal,1, glm::vec3(0, 1, 0));
		Debug::DrawRay(m_top_plane.pop,m_top_plane.normal,1, glm::vec3(0, 1, 0));
		Debug::DrawRay(m_bottom_plane.pop,m_bottom_plane.normal,1, glm::vec3(0, 1, 0));
		
		Debug::DrawLine(m_near_vert[0],m_far_vert[0]);
		Debug::DrawLine(m_near_vert[1],m_far_vert[1]);
		Debug::DrawLine(m_near_vert[2],m_far_vert[2]);
		Debug::DrawLine(m_near_vert[3],m_far_vert[3]);

		Debug::DrawLine(m_near_vert[0],m_near_vert[1]);
		Debug::DrawLine(m_near_vert[1],m_near_vert[2]);
		Debug::DrawLine(m_near_vert[2],m_near_vert[3]);
		Debug::DrawLine(m_near_vert[3],m_near_vert[0]);

		Debug::DrawLine(m_far_vert[0], m_far_vert[1]);
		Debug::DrawLine(m_far_vert[1], m_far_vert[2]);
		Debug::DrawLine(m_far_vert[2], m_far_vert[3]);
		Debug::DrawLine(m_far_vert[3], m_far_vert[0]);
		int divs = 50;
		for (int i = 0; i < divs; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				glm::vec3 dir1 = m_far_vert[j]-m_near_vert[j];
				glm::vec3 dir2 = m_far_vert[(j+1)%4] - m_near_vert[(j+1)%4];
				Debug::DrawLine(m_near_vert[j]+dir1*((float)(i+1)/divs), m_near_vert[(j+1)%4] + dir2 * ((float)(i + 1) / divs));
			}
		}
	}
};
class FrustumCuller
{
	Camera& m_cam;
	FRUSTUM m_view_frustum;
	std::vector<Entity> _render_list;
	std::shared_ptr<OctTree> m_octreee;//curr octree
	void _get_culled_in_node(OctTreeNode *node)
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
					entity_obb.max=e_det.max;
					entity_obb.min=e_det.min;
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
public:
	FrustumCuller(Camera &m_cam):m_cam(m_cam),m_view_frustum(m_cam)
	{
		
	}

	std::vector<Entity>& get_culled(std::shared_ptr<OctTree> octree)
	{
		m_view_frustum.DrawFrustum();
		m_view_frustum.CalcFrustumParameters();
		m_octreee = octree;
		_render_list.clear();
		OctTreeNode* head = octree->head;
		_get_culled_in_node(head);
		return _render_list;
	}





};
class DummyCamController:public Behaviour
{
	Camera& m_cam;
	glm::vec3 pose;
	glm::vec3 angle;
	float fov;
	float far;
public:
	AK_SHOW_IN_INSPECTOR
		(
			AK_ID_COMPX(pose)
			AK_ID_COMPX(angle)
			AK_ID_COMPX(fov)
			AK_ID_COMPX(far)
		)
	DummyCamController(Camera& m_cam) :m_cam(m_cam)
	{
		pose = glm::vec3(0);
		angle = glm::vec3(0);
		fov = 11;
		far = 1000;
	}

	void Update(float detlatime) override
	{
		m_cam.setCameraPosition(pose);
		m_cam.setCameraRotation(angle);
		m_cam.setFieldOfView(fov);
		m_cam.setFar(far);
	}
	void OnStart()override
	{
	}


};




int main()
{
	AssetManager::setAssetRoot("../../../../Assets/");
	std::string rootDir(AssetManager::getAssetRoot());
	Window window(800, 800, "DUMBSTUFF");
	
	if (!window.initialize())
	{
		std::cout << "Can't init WINDOW";
		return -1;
	}
	
	Material cmat("DEFERRED");
	cmat.setTexture2D("material.diffuse", rootDir + "Media/pbr/rust1/diffuse.png");
	cmat.setTexture2D("material.roughness", rootDir + "Media/pbr/rust1/roughness.png");
	cmat.setTexture2D("material.normal", rootDir + "Media/pbr/rust1/normal.png");
	cmat.setTexture2D("material.metallic", rootDir + "Media/pbr/rust1/metallic.png");
	cmat.setValue("noAO", 1);
	cmat.setValue("ambientocclusion", 1);
	cmat.setValue("noMetallic", 0);
	cmat.setValue("normalStrength", 1);

	Scene m_scene(window);
	
	Entity dir = m_scene.CreateEntity();
	Lights &d = m_scene.AddComponent<Lights>(dir);
	d.setType(LIGHT::DIRECTIONAL);
	d.setColor(1, 1, 1);
	d.setDirection(-90,0,0);
	d.setIntensity(0.6f);
	d.setPointLightConst(1,2,10);
	d.ambientLigting(0.1f,0.1f,0.1f);
	
	m_scene.AddComponent<Transform>(dir).SetGlobalPosition(glm::vec3(0,2,0));
	

	std::vector<Entity> cubeList;
	float m = 15;
	float dist = 100.0f;
	std::shared_ptr<OctTree> oct=std::make_shared<OctTree>(m_scene);
	float totalObj = m*m*m;
	float half = dist / 2.0f;
	float distperobj = dist/m;
	int* level=new int;
	*level=3;
	for (int i = 0; i < m*m*m; i++)
	{

		Entity cube = m_scene.CreateEntity();
		cubeList.push_back(cube);
		Transform& t = m_scene.AddComponent<Transform>(cube);
		t.SetGlobalScale(glm::vec3(2.0f));
		t.SetGlobalRotation(Quaternion(0,0,0));
		float nX = (float)((int)(i/m)%(int)(m))*distperobj-half;
		float nY = static_cast<int>(i / (m * m)) * distperobj - half;
		float nZ = static_cast<int>(i%static_cast<int>(m))*distperobj-half;
		t.SetGlobalPosition(glm::vec3(nX,nY,nZ));
		Mesh &cM=m_scene.AddComponent<Mesh>(cube);
		m_scene.AddComponent<Material>(cube)=cmat;
		cM.CreateMesh(generateCubeVertices());
		auto& beh=m_scene.AddComponent<BehaviourComponent>(cube);
		beh.setBehaviour<strafeRandom>(i%360,level);
		//if(nX>0&& nZ>0 && nY>0)

	}
	

	Material matS("SPHERE");
	Entity sky = m_scene.CreateEntity();



	m_scene.AddComponent<Transform>(sky);
	Mesh& skym = m_scene.AddComponent<Mesh>(sky);
	m_scene.AddComponent<Material>(sky) = matS;
	skym.CreateMesh(BasicShapes::quadVert, BasicShapes::quadIndices);
	/*
	Material mat("GRIDS");
	Entity plane= m_scene.CreateEntity();

	m_scene.AddComponent<Transform>(plane);
	Mesh &pm=m_scene.AddComponent<Mesh>(plane);
	m_scene.AddComponent<Material>(plane)=mat;
	pm.CreateMesh(BasicShapes::quadVert, BasicShapes::quadIndices);
	
	*/

	std::shared_ptr<RenderingSystem> rSys = m_scene.renderSys;
	

	//Editor is experimental, do not use this
	Editor edt(m_scene);
	m_scene.OnStart();
	m_scene.vsyncOn(true);
	float deltaTime = 0;
	OctTreeNode::SetMaxLevel(*level);
	for (const Entity& ent : rSys->entities)
	{
		oct->insert(ent);
	}
	Camera m_cam(60,1,0.1f,1000.0f);
	m_cam.setCameraPosition(0,0,0);
	FrustumCuller frustum(m_cam);
	auto& beh = m_scene.AddComponent<BehaviourComponent>(dir);
	beh.setBehaviour<DummyCamController>(m_cam);
	while (!window.closeWindow())
	{
		auto& list = frustum.get_culled(oct);
		for (int i = 0; i < list.size(); i++)
		{
			Mesh& m = m_scene.GetComponent<Mesh>(list[i]);
			Transform& t = m_scene.GetComponent<Transform>(list[i]);
			glm::mat4 trans = t.transformMatrix();
			Debug::DrawBB(m.GetMin(),m.GetMax(),trans,glm::vec3(1,0,0));
		}
		//oct.DrawTreeSphere();
		//oct.DrawTreeBox();
		deltaTime = m_scene.getDeltaTime();
		//flyCam(m_scene.cam,m_scene.getDeltaTime());
		//m_scene.m_cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());
		m_scene.clearBuffer();
		//m_scene.Render();
		/*
		for (auto ent : rSys->entities)
		{
			Mesh& mesh = scene.GetComponent<Mesh>(ent);
			Transform& trans = scene.GetComponent<Transform>(ent);
			Debug::DrawBB(mesh.GetMin(), mesh.GetMax(), trans.transformMatrix(), glm::vec3(0, 1, 0));
		}
		*/

		edt.DrawUI();

		
		m_scene.swapBuffers();

	}

	oct->_free();

	return 0;
}





