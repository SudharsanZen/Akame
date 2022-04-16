#pragma once
#include<Math/GlmMath.h>
#include<vector>
#include"Rendering/Camera.h"
struct FRUSTUM;
struct AABB
{
	glm::vec3 origin;
	glm::vec3 half_extent;
	glm::vec3 min;
	glm::vec3 max;
	
	AKAME_API AABB();
	AKAME_API AABB(glm::vec3 min, glm::vec3 max);
	AKAME_API void set_min_max(glm::vec3 min,glm::vec3 max);
	AKAME_API void set_half_extents_origin(glm::vec3 origin,glm::vec3 half_extent);
	AKAME_API FRUSTUM get_frustum();
	AKAME_API bool is_point_within(glm::vec3 point);
	AKAME_API bool is_sphere_within(glm::vec3 point,float radius);
	AKAME_API static bool check_aabb_intersection(const AABB& b1, const AABB& b2);
	AKAME_API bool check_aabb_intersection(const AABB& b2)const;
};
struct PLANE
{
	glm::vec3 normal;
	glm::vec3 pop;//point on plane

	AKAME_API float getDistanceOfPointFromPlane(glm::vec3 point);

	//displace the plane in the direction of normal by the given amount "dsiplace" and 
	//then get distance of the point from the plane
	AKAME_API float getDistanceOfPointFromPlane(glm::vec3 point, float displace);

};
struct VIEW_FRUSTUM
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

	AKAME_API VIEW_FRUSTUM(Camera& m_cam);

	//call this to update the details of the camera that was previously stored this object
	AKAME_API void CalcFrustumParameters();

	//call this to check if if an AABB is within the frustum 
	//(The AABB will considered/transformed into a sphere bound that encloses the whole AABB)
	AKAME_API bool check_SPHERE_Intersection(AABB& aabb);

	//draw the frustum for debuggin purpose
	AKAME_API void DrawFrustum();
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
	

	AKAME_API FRUSTUM(PLANE far_p,PLANE near_p,PLANE right_p,PLANE left_p,PLANE top_p,PLANE bottom_p);
	
	//call this to check if if an AABB is within the frustum 
	//(The AABB will considered/transformed into a sphere bound that encloses the whole AABB)
	AKAME_API bool check_SPHERE_Intersection(AABB& aabb);

	//draw the frustum for debuggin purpose
	AKAME_API void DrawFrustum();
};
struct EntityOBBDetails
{
	Entity eid;
	std::vector<glm::vec3> OBB_verts;
	glm::vec3 min, max;
	glm::vec3 position;
	glm::mat4 transform;
	bool operator == (const EntityOBBDetails& ed2)const
	{
		return eid == ed2.eid;
	}
	bool operator != (const EntityOBBDetails& ed2)const 
	{
		return eid != ed2.eid;
	}
	bool operator > (const EntityOBBDetails& ed2)const
	{
		return eid > ed2.eid;
	}
	bool operator < (const EntityOBBDetails& ed2)const
	{
		return eid < ed2.eid;
	}
};
