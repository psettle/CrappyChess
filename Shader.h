#ifndef SHADER_COMPILER_H
#define SHADER_COMPILER_H

//this header contains functions for managing files
#include"file_h.h"
#include"common.h"

/*
	This class manages an openGL shader program, geometry shaders are not supported
*/
class Shader {
public:
	/*
		Initializes the shader
		@param vShaderFile
			The fileName of the glsl file with the vertex shader code
		@param fShaderFile
			The fileName of the glsl file with the fragment shader code
	*/
	Shader(const std::string& vShaderFile, const std::string& fShaderFile);

	/*
		Tell the openGL state machine to use this shader on vertex arrays until told otherwise
	*/
	void use() const;

	/*
		Disable all shaders in the openGL state machine until told otherwise
	*/
	void clear() const;

	/*
		Get the program ID used on the shader
	*/
	GLuint getProgramID() const;

	/*
		Clears the allocated memory for this object, which means telling openGL that
		we are done with this shader
	*/
	~Shader();
private:
	/*
		The id of the compiled shader program, this is the reference openGL gives us to communicate with
	*/
	GLuint programID;

	/*
		Compile a glsl shader
		@param fileName
			The fileName of the glsl file holding the shader code
		@param type
			The open GL constant for shader types
			one of GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER
		@return
			The shaderID openGL allocates
	*/
	static GLuint compileShader(const std::string& fileName, GLuint type);

	/*
		Links a compiled vertex shader and fragment shader into a complete openGL shader
		Clears the allocated memory for the shaders after linking them
		@param vShader
			The shader id for the vertex shader to link
		@param fShader
			The shader id for the fragment shader to link
		@return
			The programID openGL allocates
	*/
	static GLuint linkProgram(const GLuint vShader, const GLuint fShader);

	/*
		Compiles and the two provided shaders and links them into a shader program
		@param vShaderFile
			The fileName of the glsl file with the vertex shader code
		@param fShaderFile
			The fileName of the glsl file with the fragment shader code
		@return
			The programID openGL allocates for the program
	*/
	static GLuint buildProgram(const std::string& vShaderFile, const std::string& fShaderFile);
};

#endif
