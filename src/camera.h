#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Camera {
public:
	Camera(const glm::vec3& pos, float fov, float aspect, float near, float far);

	glm::mat4 GetViewProjection() const;

	void Update();
	void UpdateAspect(int width, int height);

	void SetPitch(float angle);
	void SetYaw(float angle);

	float GetPitch();
	float GetYaw();
private:
	glm::mat4 m_perspective;
	glm::vec3 m_pos;

	glm::vec3 m_forward;
	glm::vec3 m_right;
	glm::vec3 m_up;

	float m_pitch;
	float m_yaw;

	float m_fov;
	float m_near;
	float m_far;
};

#endif // CAMERA_H
