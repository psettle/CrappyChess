#include"GlCamera.h"

GlCamera::GlCamera(GLFWwindow* window) {
	this->pWindowM = window;
}

glm::mat4 GlCamera::feedMatrix(const glm::mat4& source) const {
	return this->ResultCache * source;
}

void GlCamera::newView(const glm::vec3& from, const glm::vec3& to, const glm::vec3& up) {
	this->View = glm::lookAt(from, to, up);
	this->ResultCache = this->Projection * this->View;
	this->positionM = from;
}

void GlCamera::newPerspective(const float& fov, const size_t width, const size_t height, const float front, const float back) {
	float aspectRatio = (float)((double)width / (double)height);

	this->Projection = glm::perspective(fov, aspectRatio, front, back);
	this->ResultCache = this->Projection * this->View;
}

void GlCamera::passData(const std::vector<GLint> data) {}

void GlCamera::rayToWorldspace(glm::vec4& start, glm::vec4& end) const {
	glm::mat4x4 VPi = glm::inverse(this->ResultCache);

	//invert the start point
	glm::vec4 startWorld = VPi * start;
	//normalize the the coordinates with w
	startWorld = startWorld / startWorld.w;

	//invert the end point
	glm::vec4 endWorld = VPi * end;
	//normalize the the coordinates with w
	endWorld = endWorld / endWorld.w;

	//copy the data into the result containers
	start = startWorld;
	end = endWorld;
}

glm::vec3 GlCamera::getPosition() const { return this->positionM; }

void GlCamera::shutdown() {

}


GlCamera::~GlCamera() {

}