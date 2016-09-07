#include"Shader.h"


Shader::Shader(const std::string& vShaderFile, const std::string& fShaderFile) {
	//compile the shader and set the program ID as the result of the build
	this->programID = Shader::buildProgram(vShaderFile, fShaderFile);
}

void Shader::use() const {
	glUseProgram(this->programID);
}

void Shader::clear() const {
	glUseProgram(0);
}

GLuint Shader::getProgramID() const {
	return this->programID;
}

Shader::~Shader() {
	glDeleteProgram(this->programID);
}

GLuint Shader::compileShader(const std::string& fileName, GLuint type) {
	//allocate a shader
	GLuint shader;
	shader = glCreateShader(type);
	//grab the file contents
	std::string code = file_get_contents(fileName);
	const GLchar* codePtr = code.c_str();
	//pass the file contents to the shader
	glShaderSource(shader, 1, &codePtr, NULL);
	//compile the shader
	glCompileShader(shader);

	//check if the compilation was successful, print an error if it failed
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		std::cerr << "COMPILE FAILED" << std::endl;
	}
	//grab the compilation log and print it
	GLint length;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
	if (length > 1) {
		GLchar* error = new GLchar[length + 1];
		glGetShaderInfoLog(shader, length, NULL, error);
		std::cerr << "LOG: " << error << std::endl;
		delete[] error;
	}
	//return the shader ID
	return shader;

}

GLuint Shader::linkProgram(const GLuint vShader, const GLuint fShader) {
	//allocate a program
	GLuint program;
	program = glCreateProgram();
	//attach the vertex and fragment shaders
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);
	//link the program
	glLinkProgram(program);
	//check if successful and print an error on failure
	GLint success;
	glGetProgramiv(program, GL_COMPILE_STATUS, &success);
	if (!success) {
		std::cout << "COMPILE FAILED" << std::endl;
	}
	//grab the compilation log and print it
	GLint length;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
	if (length > 1) {
		GLchar* error = new GLchar[length + 1];
		glGetProgramInfoLog(program, length, NULL, error);
		std::cout << "LOG: " << error << std::endl;
		delete[] error;
	}
	//clear the allocated shader resources
	glDeleteShader(vShader);
	glDeleteShader(fShader);
	//return the program ID
	return program;
}

GLuint Shader::buildProgram(const std::string& vShaderFile, const std::string& fShaderFile) {
	//compile the vertex shader
	GLuint vertexShader = Shader::compileShader(vShaderFile, GL_VERTEX_SHADER);
	//compile the fragment shader
	GLuint fragmentShader = Shader::compileShader(fShaderFile, GL_FRAGMENT_SHADER);
	//link the programs and return the program ID
	return Shader::linkProgram(vertexShader, fragmentShader);
}