#ifndef _CAMERA_INCLUDE
#define _CAMERA_INCLUDE

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera();
	int Camerapos();
	glm::mat4 view();
	void CameraUpdate(int x);

private:
	int Cameraposx;

};

#endif // _CAMERA_INCLUDE

