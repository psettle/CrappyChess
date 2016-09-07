#ifndef GL_PLAYER_H
#define GL_PLAYER_H

#include"GlCamera.h"

class GlPlayer : public GlCamera {
public:
	GlPlayer(GLFWwindow* window)
		: GlCamera(window), position(1, 3, 3), horizontalAngle(3.14159f), verticalAngle(0.0f),
		initialFoV(45.0f), speed(3.0f), mouseSpeed(0.05f), lastTime(glfwGetTime())
	{
		int width, height;
		glfwGetWindowSize(this->pWindowM, &width, &height);
		glfwSetCursorPos(this->pWindowM, width / 2.0, height / 2.0);

	}

	virtual void frame() {
		double mouseX, mouseY;
		int width, height;
		glfwGetCursorPos(this->pWindowM, &mouseX, &mouseY);
		glfwGetWindowSize(this->pWindowM, &width, &height);
		glfwSetCursorPos(this->pWindowM, width / 2.0, height / 2.0);

		double currentTime = glfwGetTime();
		float deltaTime = float(currentTime - lastTime);
		this->lastTime = currentTime;

		this->horizontalAngle += this->mouseSpeed * deltaTime * float(width / 2.0 - mouseX);
		this->verticalAngle += this->mouseSpeed * deltaTime * float(height / 2.0 - mouseY);

		glm::vec3 direction(
			cos(this->verticalAngle) * sin(this->horizontalAngle),
			sin(this->verticalAngle),
			cos(this->verticalAngle) * cos(this->horizontalAngle)
		);

		glm::vec3 right(
			sin(this->horizontalAngle - 3.14159f / 2.0f),
			0,
			cos(this->horizontalAngle - 3.14159f / 2.0f)
		);

		glm::vec3 up(glm::cross(right, direction));

		if (glfwGetKey(this->pWindowM, GLFW_KEY_W) == GLFW_PRESS) {
			position += direction * deltaTime * this->speed;
		}

		if (glfwGetKey(this->pWindowM, GLFW_KEY_A) == GLFW_PRESS) {
			position -= right * deltaTime * this->speed;
		}

		if (glfwGetKey(this->pWindowM, GLFW_KEY_S) == GLFW_PRESS) {
			position -= direction * deltaTime * this->speed;
		}

		if (glfwGetKey(this->pWindowM, GLFW_KEY_D) == GLFW_PRESS) {
			position += right * deltaTime * this->speed;
		}
		if (glfwGetKey(this->pWindowM, GLFW_KEY_UP) == GLFW_PRESS) {
			this->initialFoV -= 0.2f;
		}
		if (glfwGetKey(this->pWindowM, GLFW_KEY_DOWN) == GLFW_PRESS) {
			this->initialFoV += 0.2f;
		}

		this->newPerspective(this->initialFoV, width, height, 0.1f, 100.0f);
		static int i = 0;
		i++;
		if (i == 500) {
			i = 0;
		}
		this->newView(position, position + direction, up);

	}
private:
	glm::vec3 position;
	float horizontalAngle;
	float verticalAngle;
	float initialFoV;
	float speed;
	float mouseSpeed;
	float lastTime;

};

#endif