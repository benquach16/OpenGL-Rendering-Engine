#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "mat4.h"
#include "vector3.h"

class Camera
{
public:

private:
	glm::mat4 m_transform;
	glm::vec3 m_origin;
	Mat4 m_transformation;
};
