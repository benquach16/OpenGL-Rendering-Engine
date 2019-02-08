#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "mat4.h"
#include "vector3.h"

class Camera
{
public:
	Camera();

	void setPosition(glm::vec3 position) { m_origin = position; }
	void lookAt();
	glm::mat4 getTransform();
private:
	glm::mat4 m_transform;
	glm::vec3 m_origin;
	//Mat4 m_transformation;
};
