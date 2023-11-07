#include "Camera.h"

Camera::Camera() {
	Cameraposx = 0;
}

int Camera::Camerapos() {
	return Cameraposx;
}

glm::mat4 Camera::view() {
	return glm::translate(glm::mat4(1.0f), glm::vec3(-Cameraposx,0.f,0.f));
}

void Camera::CameraUpdate(int x) {
	Cameraposx = x;
}