#include"Piece.h"

void Piece::setBoundingBox(const glm::vec3& lowerBound, const glm::vec3& upperBound) {
	this->box.minBound = lowerBound;
	this->box.maxBound = upperBound;
}

bool Piece::cursorIntersect(Ray ray, GLfloat& distance) const {
	if (this->box.getIntersection(this->getModel(), ray, distance, this->current_scale)) {
		return true;
	}
	return false;
}

void Piece::passUniforms() const {
	GlObject::passUniforms();

	GLuint progID = this->shader->getProgramID();
	//we need to pass
	//1 the camera position
	//2 the directional light
	//3 the material
	//4 the model matrix

	//the camera is pretty easy
	glm::vec3 cameraPos = this->camera->getPosition();
	GLuint UID = glGetUniformLocation(progID, "cameraPosition");
	glUniform3f(UID, cameraPos.x, cameraPos.y, cameraPos.z);
	
	//next the model matrix
	glm::mat4 model = this->getModel();
	UID = glGetUniformLocation(progID, "model");
	glUniformMatrix4fv(UID, 1, GL_FALSE, &model[0][0]);

	
	//and finally the directional light is all the same
	static GLuint dirDirection = glGetUniformLocation(progID, "dirLight.direction");
	static GLuint dirAmbient = glGetUniformLocation(progID, "dirLight.ambient");
	static GLuint dirDiffuse = glGetUniformLocation(progID, "dirLight.diffuse");
	static GLuint dirSpecular = glGetUniformLocation(progID, "dirLight.specular");

	//direction, in from the left
	glUniform3f(dirDirection, -2.0f, -0.3f, 0.0f);

	glUniform3f(dirAmbient, 1.0f, 1.0f, 1.0f);
	glUniform3f(dirDiffuse, 0.988f * 0.05f, 0.86f * 0.05f, 0.6f * 0.05f);
	glUniform3f(dirSpecular, 0.988f * 0.1f, 0.86f * 0.1f, 0.6f * 0.1f);

	//pass how many lights
	GLuint howManyLightsLoc = glGetUniformLocation(progID, "howManyPointLights");
	glUniform1i(howManyLightsLoc, this->pointLights->size());
	//now we need to pass all of the point lights

	for (size_t i = 0; i < this->pointLights->size() && i < 30; i++) {
		//the details to pass are
		//the lights position
		//the lights constant, linear and quadratic distance factors (i.e the equation for intensity fall)

		//the ambient strenght, the diffuse strength and the specular strength

		std::string uniformName = "lights[" + std::to_string(i) + std::string("]");

		GLuint positionLoc = glGetUniformLocation(progID, (uniformName + ".position").c_str());
		
		//pass the position
		glm::vec3 position = this->pointLights->at(i);

		glUniform3f(positionLoc, position.x, position.y, position.z);
		
	}

	static GLuint constLinQuad = glGetUniformLocation(progID, "constLinQuad");
	
	glUniform3f(constLinQuad, 0.0f, 0.0f, 50.0f);
	
	//pass the light masks in
	static GLuint lightMaskLoc = glGetUniformLocation(progID, "lightMasks");

	GLfloat masks[9] = {
		0.0f, 0.0f, 0.0f, //ambient
		3 * .776f, 3 * 1.0f, 3 * .478f, //diffuse
		2 * .776f, 2 * 1.0f, 2 * .478f, //specular
	};

	glUniform3fv(lightMaskLoc, 9, masks);
}

void Piece::setLights(std::vector<glm::vec3>* lights) {
	this->pointLights = lights;
}