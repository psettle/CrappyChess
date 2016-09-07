#include"Texture.h"

Texture::Texture(const std::string& fileName, const GLuint slot, Shader* shader, const std::string& hook_name) {
	this->slot = slot;
	this->shader = shader;
	this->hook_name = hook_name;
	GLint width, height;

	//use soil to load the image data
	unsigned char* image = SOIL_load_image(fileName.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);

	//generate and bind a texture
	glGenTextures(1, &this->textureID);
	glBindTexture(GL_TEXTURE_2D, this->textureID);

	//tell open gl how to scale and alias the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//bind the image data to the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	//generate a MIPMAP for the texture
	glGenerateMipmap(GL_TEXTURE_2D);

	//safely delete the image data
	SOIL_free_image_data(image);

	//clear the bound texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::use() const {
	glActiveTexture(this->slot);
	glBindTexture(GL_TEXTURE_2D, this->textureID);
	GLuint UID = glGetUniformLocation(this->shader->getProgramID(), this->hook_name.c_str());
	glUniform1i(UID, this->slot - GL_TEXTURE0);
}

void Texture::clear() const {
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(0);
}

Texture::~Texture() {
	shader = NULL;
	glDeleteTextures(1, &this->textureID);
}