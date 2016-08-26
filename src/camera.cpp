#include "camera.h"

Camera::Camera(const glm::vec3& pos, float fov, float aspect, float near, float far)
{
	m_fov = fov;
	m_near = near;
	m_far = far;
	m_perspective = glm::perspective(fov, aspect, near, far);
	m_pos = pos;
	m_pos;

	Update();
}

void Camera::Update()
{
	// Calculate the new Front vector
	glm::vec3 forward;
	forward.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	forward.y = sin(glm::radians(m_pitch));
	forward.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_forward = glm::normalize(forward);
	// Also re-calculate the Right and Up vector
	m_right = glm::normalize(glm::cross(m_forward, glm::vec3(0, 1, 0)));

	m_up = glm::normalize(glm::cross(m_right, m_forward));
}

void Camera::UpdateAspect(int width, int height)
{
	m_perspective = glm::perspective(m_fov, (float)width/(float)height, m_near, m_far);
}

glm::mat4 Camera::GetViewProjection() const
{
	return m_perspective * glm::lookAt(m_pos, m_pos + m_forward, m_up);
}

void Camera::SetPitch(float angle)
{
	if (angle > 89.0f) {
		m_pitch = 89.0f;
	}
	else if (angle < -89.0f) {
		m_pitch = -89.0f;
	} else {
		m_pitch = angle;
	}
}

void Camera::SetYaw(float angle)
{
	m_yaw = angle;
}

float Camera::GetPitch()
{
	return m_pitch;
}

float Camera::GetYaw()
{
	return m_yaw;
}
