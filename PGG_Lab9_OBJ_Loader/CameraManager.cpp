#include "CameraManager.h"


CameraManager::CameraManager()
{
	matRoll = glm::mat4(1.0f);//identity matrix; 
	matPitch = glm::mat4(1.0f);//identity matrix
	matYaw = glm::mat4(1.0f);//identity matrix
}


CameraManager::~CameraManager()
{
}

void CameraManager::UpdateView(float roll, float pitch, float yaw)
{
	matRoll = glm::mat4(1.0f);//identity matrix; 
	matPitch = glm::mat4(1.0f);//identity matrix
	matYaw = glm::mat4(1.0f);//identity matrix

	matRoll = glm::rotate(matRoll, roll, glm::vec3(0.0f, 0.0f, 1.0f));
	matPitch = glm::rotate(matPitch, pitch, glm::vec3(1.0f, 0.0f, 0.0f));
	matYaw = glm::rotate(matYaw, yaw, glm::vec3(0.0f, 1.0f, 0.0f));	

	glm::mat4 rotate = matRoll * matPitch * matYaw;

	glm::mat4 translate = glm::mat4(1.0f);
	//translate = glm::translate(translate, -eyeVector);

	//viewMatrix = rotate * translate;
}
