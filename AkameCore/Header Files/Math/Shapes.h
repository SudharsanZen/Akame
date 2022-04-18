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
	
	inline AABB()
	{
		origin = glm::vec3(0);
		half_extent = glm::vec3(0);
		min = glm::vec3(0);
		max = glm::vec3(0);

	}
	inline AABB(glm::vec3 min, glm::vec3 max)
	{
		set_min_max(min, max);
	}
	inline void set_min_max(glm::vec3 min, glm::vec3 max)
	{
		this->min = min;
		this->max = max;
		origin = (max + min) / 2.0f;
		half_extent = glm::abs(max - origin);
	}
	inline void set_half_extents_origin(glm::vec3 origin, glm::vec3 half_extent)
	{
		this->origin = origin;
		this->half_extent = half_extent;

		min = origin - half_extent;
		max = origin + half_extent;
	}
	AKAME_API FRUSTUM get_frustum();
	inline bool is_point_within(glm::vec3 point)
	{
		return
			(
				point.x <= max.x && point.x >= min.x &&
				point.y <= max.y && point.y >= min.y &&
				point.z <= max.z && point.z >= min.z
				);
	}
	AKAME_API bool is_sphere_within(glm::vec3 point,float radius);
	inline static bool check_aabb_intersection(const AABB& b1, const AABB& b2)
	{
		return
			(
				fabs(b1.origin.x - b2.origin.x) < b1.half_extent.x + b2.half_extent.x &&
				fabs(b1.origin.y - b2.origin.y) < b1.half_extent.y + b2.half_extent.y &&
				fabs(b1.origin.z - b2.origin.z) < b1.half_extent.z + b2.half_extent.z
				);
	}
	inline bool check_aabb_intersection(const AABB& b2)const
	{
		return
			(
				fabs(origin.x - b2.origin.x) < half_extent.x + b2.half_extent.x &&
				fabs(origin.y - b2.origin.y) < half_extent.y + b2.half_extent.y &&
				fabs(origin.z - b2.origin.z) < half_extent.z + b2.half_extent.z
				);
	}
};
struct PLANE
{
	glm::vec3 normal;
	glm::vec3 pop;//point on plane
	inline bool check_AABB_intersection(const AABB& aabb)
	{

		return ((fabs(aabb.half_extent.x * normal.x) +
			fabs(aabb.half_extent.y * normal.y) +
			fabs(aabb.half_extent.z * normal.z)) >= fabs(getDistanceOfPointFromPlane(aabb.origin)));
	}

	inline bool check_if_AABB_Behind_Or_Intersecting(const AABB& aabb)
	{
		float dist_origin = getDistanceOfPointFromPlane(aabb.origin);
		return (dist_origin<=0 || (fabs(aabb.half_extent.x * normal.x) +
				 fabs(aabb.half_extent.y * normal.y )+
				 fabs(aabb.half_extent.z * normal.z)) >= fabs(dist_origin));
	}
	inline float getDistanceOfPointFromPlane(const glm::vec3& point)
	{
		return glm::dot(normal, point - pop);
	}

	//displace the plane in the direction of normal by the given amount "diplace" and 
	//then get distance of the point from the plane
	inline float getDistanceOfPointFromPlane(const glm::vec3 &point, float displace)
	{
		return glm::dot(normal, point - (pop + normal * displace));
	}

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
	inline bool VIEW_FRUSTUM::check_SPHERE_Intersection(const AABB& aabb)
	{
		glm::vec3 origin = (aabb.max + aabb.min) / 2.0f;
		float radius = glm::length((aabb.max - aabb.min)) / 2.0f;
		//Debug::DrawCircle(origin, worldLeft, 0.2f, glm::vec3(0, 1, 0));
		return	(m_bottom_plane.getDistanceOfPointFromPlane(origin, radius) < 0) &&
			(m_top_plane.getDistanceOfPointFromPlane(origin, radius) < 0) &&
			(m_left_plane.getDistanceOfPointFromPlane(origin, radius) < 0) &&
			(m_right_plane.getDistanceOfPointFromPlane(origin, radius) < 0) &&
			(m_far_plane.getDistanceOfPointFromPlane(origin, radius) < 0) &&
			(m_near_plane.getDistanceOfPointFromPlane(origin, radius) < 0);
	}
	//sphere-frustum intersection or collision check
	inline bool check_SPHERE_Intersection(glm::vec3 &&origin,float radius)
	{
		//Debug::DrawCircle(origin, worldLeft, 0.2f, glm::vec3(0, 1, 0));
		return	(m_bottom_plane.getDistanceOfPointFromPlane(origin, radius) < 0) &&
			(m_top_plane.getDistanceOfPointFromPlane(origin, radius) < 0) &&
			(m_left_plane.getDistanceOfPointFromPlane(origin, radius) < 0) &&
			(m_right_plane.getDistanceOfPointFromPlane(origin, radius) < 0) &&
			(m_far_plane.getDistanceOfPointFromPlane(origin, radius) < 0) &&
			(m_near_plane.getDistanceOfPointFromPlane(origin, radius) < 0);
	}


	inline bool check_AABB_Intersection(const AABB& aabb)
	{
		
		return
			(
				m_far_plane.check_if_AABB_Behind_Or_Intersecting(aabb) &&
				m_near_plane.check_if_AABB_Behind_Or_Intersecting(aabb)&&
				m_right_plane.check_if_AABB_Behind_Or_Intersecting(aabb)&&
				m_left_plane.check_if_AABB_Behind_Or_Intersecting(aabb) &&
				m_top_plane.check_if_AABB_Behind_Or_Intersecting(aabb)&&
				m_bottom_plane.check_if_AABB_Behind_Or_Intersecting(aabb)
			);
	}

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
	inline bool check_SPHERE_Intersection(AABB& aabb)
	{
		glm::vec3 origin = (aabb.max + aabb.min) / 2.0f + aabb.origin;
		float radius = glm::length((aabb.max - aabb.min)) / 2.0f;
		//Debug::DrawCircle(origin, worldLeft, 0.2f, glm::vec3(0, 1, 0));
		return	(m_bottom_plane.getDistanceOfPointFromPlane(origin, radius) < 0) &&
			(m_top_plane.getDistanceOfPointFromPlane(origin, radius) < 0) &&
			(m_left_plane.getDistanceOfPointFromPlane(origin, radius) < 0) &&
			(m_right_plane.getDistanceOfPointFromPlane(origin, radius) < 0) &&
			(m_far_plane.getDistanceOfPointFromPlane(origin, radius) < 0) &&
			(m_near_plane.getDistanceOfPointFromPlane(origin, radius) < 0);
	}

	//draw the frustum for debuggin purpose
	AKAME_API void DrawFrustum();
};
struct EntityOBBDetails
{
	Entity eid;
	std::vector<glm::vec3> OBB_verts;
	glm::vec3 position;
	glm::mat4 transform;
	AABB aabb;
	inline bool operator == (const EntityOBBDetails& ed2)const
	{
		return eid == ed2.eid;
	}
	inline bool operator != (const EntityOBBDetails& ed2)const 
	{
		return eid != ed2.eid;
	}
	inline bool operator > (const EntityOBBDetails& ed2)const
	{
		return eid > ed2.eid;
	}
	inline bool operator < (const EntityOBBDetails& ed2)const
	{
		return eid < ed2.eid;
	}
};
