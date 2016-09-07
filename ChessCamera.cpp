#include"ChessCamera.h"

ChessCamera::ChessCamera(const GLuint width, const GLuint height)
	: GlCamera(), angle(0.0f), colorToLookAt(WHITE), lastFrameTime((GLfloat)glfwGetTime()) {
	this->newPerspective(PI / 3, width, height, 0.1f, 100.0f);
	this->newView(glm::vec3(0.0f, HEIGHT, - RADIUS), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void ChessCamera::frame() {
	//we always grab the current time
	//and set it as the last time, otherwise the
	//camera executes a full loop immediately upon the request
	GLdouble currentTime = glfwGetTime();
	GLdouble dTime = currentTime - this->lastFrameTime;
	this->lastFrameTime = currentTime;

	//reset the glfw timer if it gets too high (we lose percision)
	if (currentTime > 3600) {
		glfwSetTime(0.0);
		this->lastFrameTime = 0.0;
	}

	//if the animation isn't running stop
	if (!this->animationRunning) {
		return;
	}

	//we want the rotation to take about 2s, so we'll calculate the time since the last frame
	//and use that to calculate the angle differentials
		
	//now what fraction of rotation time is the dTime?
	GLdouble fracRotation = dTime / ROTATION_TIME;

	GLdouble dAngle = PI_D * fracRotation;

	//increment/decrement the angle
	if (this->colorToLookAt) {
		this->angle -= dAngle;
		//if the angle goes below 0, cap it, and end the animation
		if (this->angle < 0.0) {
			this->animationRunning = GL_FALSE;
			this->angle = 0.0;
		}
	}
	else {
		this->angle += dAngle;
		//if the angle goes past PI, cap it, and end the animation
		if (this->angle > PI_D) {
			this->animationRunning = GL_FALSE;
			this->angle = PI_D;
		}
	}
	//the distance from the origin to the camera on the xz plane is always RADIUS
	GLdouble cosAngle = glm::cos(this->angle);
		
	GLdouble zComponent = -cosAngle * RADIUS;

	GLdouble sinAngle = glm::sin(this->angle);

	GLdouble xComponent = sinAngle * RADIUS;

	this->newView(glm::vec3(xComponent, HEIGHT, zComponent), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void ChessCamera::passData(const std::vector<GLint> data) {
	this->lookAt(data[0]);
}

void ChessCamera::lookAt(GLboolean color) {
	this->colorToLookAt = color;
	this->animationRunning = GL_TRUE;
}