#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Transform
{
public:
	Transform(const glm::vec3 &pos = glm::vec3(),
			  const glm::vec3 &rot = glm::vec3(),
			  const glm::vec3 &scale = glm::vec3(1.0, 1.0, 1.0)) :
		m_pos(pos),
		m_rot(rot),
		m_scale(scale) {}

	glm::mat4 GetModel() const;

	glm::vec3& GetPos();
	glm::vec3& GetRot();
	glm::vec3& GetScale();

	void SetPos(const glm::vec3 &pos);
	void SetRot(const glm::vec3 &rot);
	void SetScale(const glm::vec3 &scale);
private:
	glm::vec3 m_pos;
	glm::vec3 m_rot;
	glm::vec3 m_scale;
};

#endif // TRANSFORM_H
