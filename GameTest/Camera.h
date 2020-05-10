#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class Camera {
private:
	glm::vec3 position{ 4,3,3 };
	glm::vec3 lookAt{ 0,0,0 };
	glm::vec3 up{ 0,1,0 };
public:
	glm::mat4 GetLookAtMatrix() {
		glm::mat4 View = glm::lookAt(
			position,
			lookAt,		
			up
		);
		return View;
	}
};