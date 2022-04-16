#include "Math/Shapes.h"
#include"Core/Debug/Debug.h"

#define SQUARE(x) (x)*(x) 
float PLANE::getDistanceOfPointFromPlane(glm::vec3 point)
{
	return glm::dot(normal, point - pop);
}

float PLANE::getDistanceOfPointFromPlane(glm::vec3 point, float displace)
{
	return glm::dot(normal, point - (pop + normal * displace));
}

VIEW_FRUSTUM::VIEW_FRUSTUM(Camera& m_cam) :m_near_vert(4), m_far_vert(4), m_camera(m_cam)
{
	CalcFrustumParameters();
}

void VIEW_FRUSTUM::CalcFrustumParameters()
{
	m_fov = m_camera.getFov();

	m_camera_pose = m_camera.getCameraPosition();
	m_aspect_ratio = m_camera.getAspectRatio();
	m_near = m_camera.getNear();
	m_far = m_camera.getFar();

	float near_height = glm::tan(m_fov / 2.0f) * m_near;
	float near_width = m_aspect_ratio * near_height;
	float far_height = glm::tan(m_fov) * m_far;
	float far_width = m_aspect_ratio * far_height;

	glm::vec3 right = m_camera.transform.right();
	glm::vec3 up = m_camera.transform.up();
	glm::vec3 forward = m_camera.transform.forward();

	m_near_plane.normal = -forward;
	m_far_plane.normal = forward;

	m_near_plane.pop = m_camera_pose + forward * m_near;
	m_far_plane.pop = m_camera_pose + forward * m_far;
	//the points are calculated in clockwise order starting from the point in positive quadrant
	m_near_vert[0] = forward * m_near + m_camera_pose + right * near_width + up * near_height;
	m_near_vert[1] = forward * m_near + m_camera_pose + right * near_width - up * near_height;
	m_near_vert[2] = forward * m_near + m_camera_pose - right * near_width - up * near_height;
	m_near_vert[3] = forward * m_near + m_camera_pose - right * near_width + up * near_height;
	//the points are calculated in clockwise order starting from the point in positive quadrant
	m_far_vert[0] = forward * m_far + m_camera_pose + right * far_width + up * far_height;
	m_far_vert[1] = forward * m_far + m_camera_pose + right * far_width - up * far_height;
	m_far_vert[2] = forward * m_far + m_camera_pose - right * far_width - up * far_height;
	m_far_vert[3] = forward * m_far + m_camera_pose - right * far_width + up * far_height;

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


bool VIEW_FRUSTUM::check_SPHERE_Intersection(AABB& aabb)
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

void VIEW_FRUSTUM::DrawFrustum()
{
	Debug::DrawRay(m_near_plane.pop, m_near_plane.normal, 1, glm::vec3(0, 1, 0));
	Debug::DrawRay(m_far_plane.pop, m_far_plane.normal, 1, glm::vec3(0, 1, 0));
	Debug::DrawRay(m_right_plane.pop, m_right_plane.normal, 1, glm::vec3(0, 1, 0));
	Debug::DrawRay(m_left_plane.pop, m_left_plane.normal, 1, glm::vec3(0, 1, 0));
	Debug::DrawRay(m_top_plane.pop, m_top_plane.normal, 1, glm::vec3(0, 1, 0));
	Debug::DrawRay(m_bottom_plane.pop, m_bottom_plane.normal, 1, glm::vec3(0, 1, 0));

	Debug::DrawLine(m_near_vert[0], m_far_vert[0]);
	Debug::DrawLine(m_near_vert[1], m_far_vert[1]);
	Debug::DrawLine(m_near_vert[2], m_far_vert[2]);
	Debug::DrawLine(m_near_vert[3], m_far_vert[3]);

	Debug::DrawLine(m_near_vert[0], m_near_vert[1]);
	Debug::DrawLine(m_near_vert[1], m_near_vert[2]);
	Debug::DrawLine(m_near_vert[2], m_near_vert[3]);
	Debug::DrawLine(m_near_vert[3], m_near_vert[0]);

	Debug::DrawLine(m_far_vert[0], m_far_vert[1]);
	Debug::DrawLine(m_far_vert[1], m_far_vert[2]);
	Debug::DrawLine(m_far_vert[2], m_far_vert[3]);
	Debug::DrawLine(m_far_vert[3], m_far_vert[0]);
	int divs = 10;
	for (int i = 0; i < divs; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			glm::vec3 dir1 = m_far_vert[j] - m_near_vert[j];
			glm::vec3 dir2 = m_far_vert[(j + 1) % 4] - m_near_vert[(j + 1) % 4];
			Debug::DrawLine(m_near_vert[j] + dir1 * ((float)(i + 1) / divs), m_near_vert[(j + 1) % 4] + dir2 * ((float)(i + 1) / divs));
		}
	}
}

FRUSTUM::FRUSTUM(PLANE far_p, PLANE near_p, PLANE right_p, PLANE left_p, PLANE top_p, PLANE bottom_p)
{
	m_far_plane = far_p;
	m_near_plane = near_p;
	m_right_plane = right_p;
	m_left_plane = left_p;
	m_top_plane = top_p;
	m_bottom_plane = bottom_p;
}
void FRUSTUM::DrawFrustum()
{
	Debug::DrawRay(m_near_plane.pop, m_near_plane.normal, 1, glm::vec3(0, 1, 0));
	Debug::DrawRay(m_far_plane.pop, m_far_plane.normal, 1, glm::vec3(0, 1, 0));
	Debug::DrawRay(m_right_plane.pop, m_right_plane.normal, 1, glm::vec3(0, 1, 0));
	Debug::DrawRay(m_left_plane.pop, m_left_plane.normal, 1, glm::vec3(0, 1, 0));
	Debug::DrawRay(m_top_plane.pop, m_top_plane.normal, 1, glm::vec3(0, 1, 0));
	Debug::DrawRay(m_bottom_plane.pop, m_bottom_plane.normal, 1, glm::vec3(0, 1, 0));

	Debug::DrawLine(m_near_vert[0], m_far_vert[0]);
	Debug::DrawLine(m_near_vert[1], m_far_vert[1]);
	Debug::DrawLine(m_near_vert[2], m_far_vert[2]);
	Debug::DrawLine(m_near_vert[3], m_far_vert[3]);

	Debug::DrawLine(m_near_vert[0], m_near_vert[1]);
	Debug::DrawLine(m_near_vert[1], m_near_vert[2]);
	Debug::DrawLine(m_near_vert[2], m_near_vert[3]);
	Debug::DrawLine(m_near_vert[3], m_near_vert[0]);

	Debug::DrawLine(m_far_vert[0], m_far_vert[1]);
	Debug::DrawLine(m_far_vert[1], m_far_vert[2]);
	Debug::DrawLine(m_far_vert[2], m_far_vert[3]);
	Debug::DrawLine(m_far_vert[3], m_far_vert[0]);
	int divs = 10;
	for (int i = 0; i < divs; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			glm::vec3 dir1 = m_far_vert[j] - m_near_vert[j];
			glm::vec3 dir2 = m_far_vert[(j + 1) % 4] - m_near_vert[(j + 1) % 4];
			Debug::DrawLine(m_near_vert[j] + dir1 * ((float)(i + 1) / divs), m_near_vert[(j + 1) % 4] + dir2 * ((float)(i + 1) / divs));
		}
	}
}
bool FRUSTUM::check_SPHERE_Intersection(AABB& aabb)
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
AABB::AABB()
{
	origin = glm::vec3(0);
	half_extent = glm::vec3(0);
	min = glm::vec3(0);
	max = glm::vec3(0);

}

AABB::AABB(glm::vec3 min, glm::vec3 max)
{
	set_min_max(min, max);
}

void AABB::set_min_max(glm::vec3 min, glm::vec3 max)
{
	this->min = min;
	this->max = max;
	origin = (max+min)/2.0f;
	half_extent = glm::abs(max-origin);
}

void AABB::set_half_extents_origin(glm::vec3 origin, glm::vec3 half_extent)
{
	this->origin = origin;
	this->half_extent=half_extent;

	min = origin-half_extent;
	max = origin+half_extent;
}

FRUSTUM AABB::get_frustum()
{
	PLANE far_p = {
			worldForward,
			max
	};
	PLANE near_p= {
		-far_p.normal,
			min
	};
	PLANE right_p = {
			worldRight,
			max
	};
	PLANE left_p = {
			-right_p.normal,
			min
	};
	PLANE top_p= {
		worldUp,
			max
	};
	PLANE bottom_p = {
		-top_p.normal,
		min
	};
	return FRUSTUM(far_p,near_p,right_p,left_p,top_p,bottom_p);
}

bool AABB::is_point_within(glm::vec3 point)
{
	return
		(
			point.x <= max.x && point.x >= min.x &&
			point.y <= max.y && point.y >= min.y &&
			point.z <= max.z && point.z >= min.z
		);
}

bool AABB::is_sphere_within(glm::vec3 point,float r)
{
	float sqaured_dist = 0;

	sqaured_dist  = (point.x < min.x) ? SQUARE(point.x - min.x) :( (point.x > min.x) ? SQUARE(point.x-max.x) : 0);
	sqaured_dist += (point.y < min.y) ? SQUARE(point.y - min.y) :( (point.y > min.y) ? SQUARE(point.y-max.y) : 0);
	sqaured_dist += (point.z < min.z) ? SQUARE(point.z - min.z) :( (point.z > min.z) ? SQUARE(point.z-max.z) : 0);
	return (sqaured_dist<=SQUARE(r));
}

bool AABB::check_aabb_intersection(const AABB& b1, const AABB& b2)
{
	return
		(
			abs(b1.origin.x-b2.origin.x) < b1.half_extent.x+b2.half_extent.x &&
			abs(b1.origin.y-b2.origin.y) < b1.half_extent.y+b2.half_extent.y &&
			abs(b1.origin.z-b2.origin.z) < b1.half_extent.z+b2.half_extent.z
		);
}

bool AABB::check_aabb_intersection(const AABB& b2) const
{
	return
		(
			abs(origin.x - b2.origin.x) < half_extent.x + b2.half_extent.x &&
			abs(origin.y - b2.origin.y) < half_extent.y + b2.half_extent.y &&
			abs(origin.z - b2.origin.z) < half_extent.z + b2.half_extent.z
		);
}
