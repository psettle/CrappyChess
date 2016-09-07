#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H

#include"common.h"
#include"Shader.h"

/*
	This is a helper class for managing textures
*/
class Texture {
public:
	/*
		Initializes the texture
		@param fileName
			The name of the image file to use
		@param slot
			The hardware slot to load the texture into, options are GL_TEXTURE0 -> GL_TEXTURE31
		@param shader
			A pointer to the shader this texture will be used with,
			this class will not delete shader
		@param hook_name
			The name of the uniform variable that will recieve the texture in the provided shader
	*/
	Texture(const std::string& fileName, const GLuint slot, Shader* shader, const std::string& hook_name);

	/*
		Sets this texture as active until openGL is told otherwise
	*/
	void use() const;

	/*
		Clears all bound texture data from openGL
	*/
	void clear() const;
	/*
		Safely de-allocates all allocated resources
	*/
	~Texture();
private:
	/*
		The texture ID openGL allocates
	*/
	GLuint textureID;

	/*
		The slot this texture is bound to
	*/
	GLuint slot;

	/*
		A pointer to the shader this texture will be used with
	*/
	Shader* shader;

	/*
		The name of the uniform variable the texture data will be passed to
	*/
	std::string hook_name;
};

#endif