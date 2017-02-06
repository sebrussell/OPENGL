#pragma once
#include "glm.hpp"
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

class CameraManager
{
public:
	CameraManager();
	~CameraManager();
	void UpdateView(float roll, float pitch, float yaw);
private:
	glm::mat4 matRoll, matPitch, matYaw;
};

