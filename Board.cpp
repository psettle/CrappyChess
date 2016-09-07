#include"Board.h"

Board::Board()
	: Piece()
{
	ObjBinFile file("models/board.objbin");

	this->addVertices(file.getVertexes());
	this->addUVs(file.getUVs());
	this->addNormals(file.getNormals());
	//this->setShaders("shaders/directionalLightVertexShader.glsl", "shaders/directionalLightFragmentShader.glsl");
	//this->setTexture("textures/boardTexture.png");
	this->scale(1.6f);
	this->rotate(PI / 2, glm::vec3(0.0f, 1.0f, 0.0f));

	//the board has a huge bounding box so we always get an intersect
	this->setBoundingBox(glm::vec3(-1000.0f, 0.0f, -1000.0f), glm::vec3(1000.0f, 0.154179, 1000.0f));
}

GLfloat Board::getTopHeight() const {
	return 0.154179f * this->current_scale;
}

GLfloat Board::getWidth() const {
	return 1.206948f * 2 * this->current_scale;
}

void Board::passUniforms() const {
	Piece::passUniforms();
	GLuint progID = this->shader->getProgramID();
	//just pass the material here
	//the material for these pieces is all the same
	GLuint matAmbientLoc = glGetUniformLocation(progID, "material.ambient");
	GLuint matDiffuseLoc = glGetUniformLocation(progID, "material.diffuse");
	GLuint matSpecularLoc = glGetUniformLocation(progID, "material.specular");
	GLuint matShineLoc = glGetUniformLocation(progID, "material.shininess");

	glUniform3f(matAmbientLoc, 0.5f, 0.5f, 0.5f);
	glUniform3f(matDiffuseLoc, 0.2f, 0.2f, 0.2f);
	glUniform3f(matSpecularLoc, 1.0f, 1.0f, 1.0f);
	glUniform1f(matShineLoc, 1.0f);
}