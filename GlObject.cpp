#include"GlObject.h"

GlObject::GlObject() :
	vertices(), colors(), uvs(), vertexShader(),
	fragmentShader(), VAO(0), buffers(), normals(),
	shader(NULL), camera(NULL), Model(1.0f), current_scale(1.0f),
	current_position(0.0f), texture(NULL), rotation(glm::rotate(0.0f, glm::vec3(1.0f))){}

void GlObject::addVertices(const std::vector<GLfloat>& floats) {
	this->vertices = floats;
}

void GlObject::addUVs(const std::vector<GLfloat>& UVs) {
	this->uvs = UVs;
}

void GlObject::addColors(const std::vector<GLfloat>& Colors) {
	this->colors = Colors;
}

void GlObject::addNormals(const std::vector <GLfloat>& Normals) {
	this->normals = Normals;
}

void GlObject::setShader(const std::string& vertexShader, const std::string& fragmentShader) {
	this->shader = new Shader(vertexShader, fragmentShader);
	this->vertexShader = vertexShader;
	this->fragmentShader = fragmentShader;
}

void GlObject::setShader(Shader* shader) {
	this->shader = shader;
}

void GlObject::setCamera(GlCamera* camera) {
	this->camera = camera;
}

void GlObject::setTexture(const std::string& fileName) {
	this->textureFile = fileName;
}

void GlObject::setTexture(Texture* texture) {
	this->texture = texture;
}

void GlObject::initialize() {
	//if a texture file is set, load the texture
	if (this->textureFile != "") {
		this->texture = new Texture(this->textureFile, GL_TEXTURE0, this->shader, "image1");
	}

	GLuint CBO, VBO, UVO, NBO;
	//generate a vertex array and buffers for each array we need to pass
	glGenVertexArrays(1, &this->VAO);
	//color buffer
	glGenBuffers(1, &CBO);
	//vertex buffer
	glGenBuffers(1, &VBO);
	//uv buffer
	glGenBuffers(1, &UVO);
	//normal buffer
	glGenBuffers(1, &NBO);

	//add all the new buffers to the list of buffers to delete
	this->buffers.push_back(CBO);
	this->buffers.push_back(VBO);
	this->buffers.push_back(UVO);
	this->buffers.push_back(NBO);

	//set the vertex array to bind to
	glBindVertexArray(VAO);

	//bind ab array byffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//load the points into the buffer
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(GLfloat), &this->vertices[0], GL_STATIC_DRAW);
	//tell openGL what kind of data we passed
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//bind the buffer to the vertex array object
	glEnableVertexAttribArray(0);

	if (this->colors.size() != 0) {
		glBindBuffer(GL_ARRAY_BUFFER, CBO);
		glBufferData(GL_ARRAY_BUFFER, this->colors.size() * sizeof(GLfloat), &this->colors[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(1);
	}

	if (this->uvs.size() != 0) {
		glBindBuffer(GL_ARRAY_BUFFER, UVO);
		glBufferData(GL_ARRAY_BUFFER, this->uvs.size() * sizeof(GLfloat), &this->uvs[0], GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(2);
	}

	if (this->normals.size() != 0) {
		glBindBuffer(GL_ARRAY_BUFFER, NBO);
		glBufferData(GL_ARRAY_BUFFER, this->normals.size() * sizeof(GLfloat), &this->normals[0], GL_STATIC_DRAW);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(3);
	}

	

	//clear the vertex array object
	glBindVertexArray(0);
	//clear the array buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GlObject::draw() const {
	
	
	//set the shader and texture
	this->shader->use();
	if (this->texture != NULL) {
		this->texture->use();
	}
	//pass data to the shaders
	this->passUniforms();

	//set the vertex array object active
	glBindVertexArray(this->VAO);
	//draw the triangles
	glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
	//clear the array object
	glBindVertexArray(0);

	//clear the texture and shader
	if (this->texture != NULL) {
		this->texture->clear();
	}

	this->shader->clear();
}

void GlObject::passUniforms() const {
	glm::mat4 MVP = this->camera->feedMatrix(this->getModel());
	GLuint UID = glGetUniformLocation(this->shader->getProgramID(), "MVP");
	glUniformMatrix4fv(UID, 1, GL_FALSE, &MVP[0][0]);
}

void GlObject::rotate(const GLfloat angle, const glm::vec3& axis) {
	this->rotation *= glm::rotate(angle, axis);
}

void GlObject::translate(const glm::vec3& direction) {
	this->Model *= glm::translate(direction);
	this->current_position += direction;
}

void GlObject::scale(const GLfloat factor) {
	this->current_scale = factor;
}

void GlObject::position(const glm::vec3& position) {
	//move the object to the world center
	this->translate(-1.0f * this->current_position);
	//translate the object to the requested position
	this->translate(position);
}

glm::vec3 GlObject::getPosition() const {
	return this->current_position;
}

GLfloat GlObject::getScale() const {
	return this->current_scale;
}

glm::vec3 GlObject::worldToModel(const glm::vec3& worldVector) const {
	glm::mat4x4 Modeli = glm::inverse(this->getModel());//this->Model * glm::scale(glm::vec3(this->current_scale)));

	glm::vec4 modelPos = Modeli * glm::vec4(worldVector, 1.0f);
	//normalize the position
	return glm::vec3(modelPos / modelPos.w);
}

glm::vec3 GlObject::modelToWorld(const glm::vec3& modelVector) const {
	return glm::vec3(this->getModel() * glm::vec4(modelVector, 1.0f));
}

glm::mat4 GlObject::getModel() const {
	return this->Model * glm::scale(glm::vec3(this->current_scale)) * this->rotation;
}

GlObject::~GlObject() {
	glDeleteBuffers(this->buffers.size(), &this->buffers[0]);
	glDeleteVertexArrays(1, &this->VAO);
	
	if (this->textureFile != "") {
		delete this->texture;
	}

	if (this->vertexShader != "" || this->fragmentShader != "") {
		delete this->shader;
	}
	
}