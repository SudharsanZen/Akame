#include "Math/Shapes.h"
#include"Core/Debug/Debug.h"
float PLANE::getDistanceOfPointFromPlane(glm::vec3 point)
{
	return glm::dot(normal, point - pop);
}

float PLANE::getDistanceOfPointFromPlane(glm::vec3 point, float displace)
{
	return glm::dot(normal, point - (pop + normal * displace));
}

FRUSTUM::FRUSTUM(Camera& m_cam) :m_near_vert(4), m_far_vert(4), m_camera(m_cam)
{
	CalcFrustumParameters();
}

void FRUSTUM::CalcFrustumParameters()
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
