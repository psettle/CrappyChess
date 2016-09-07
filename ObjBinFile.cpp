#include"ObjBinFile.h"

ObjBinFile::ObjBinFile() {}

std::vector<GLfloat> ObjBinFile::getVertexes() const {
	return this->vertexes;
}

std::vector<GLfloat> ObjBinFile::getNormals() const {
	return this->normals;
}

std::vector<GLfloat> ObjBinFile::getUVs() const {
	return this->uvs;
}

ObjBinFile::ObjBinFile(const std::string& fileName) {
	//grab the file contents
	std::vector<char> contents = binary_get_contents(fileName);
	//initialize the offset
	GLuint offset = 0;

	//load each vector from the contents
	offset = this->loadVector(this->vertexes, contents, offset);
	offset = this->loadVector(this->uvs, contents, offset);
	offset = this->loadVector(this->normals, contents, offset);
}

void ObjBinFile::loadObjFile(const std::string& fileName) {
	//load the obj file
	ObjFile file(fileName);
	//assign the members
	this->vertexes = file.getVertices();
	this->normals = file.getNormals();
	this->uvs = file.getUVs();
}

void ObjBinFile::printVector(std::vector<char>& contents, const std::vector<GLfloat>& vector) const {
	//first we print out the number of floats
	GLuint floatCount = vector.size();
	//and add that to the contents
	char* target = (char*)&floatCount;
	for (size_t i = 0; i < sizeof(GLuint); i++) {
		contents.push_back(target[i]);
	}

	//next we print every float in the vertex array
	for (size_t i = 0; i < floatCount; i++) {

		GLfloat toPrint = vector[i];

		target = (char*)&toPrint;
		for (size_t j = 0; j < sizeof(GLfloat); j++) {
			contents.push_back(target[j]);
		}
	}
}

void ObjBinFile::saveObjBinFile(const std::string& fileName) const {
	//the vector to hold the binary data
	std::vector<char> contents;
	//print each vector into the vector
	this->printVector(contents, this->vertexes);
	this->printVector(contents, this->uvs);
	this->printVector(contents, this->normals);
	//save the file contents
	binary_put_contents(fileName, contents);
}

GLuint ObjBinFile::loadVector(std::vector<GLfloat>& vector, const std::vector<char>& contents, GLuint offset) {
	//first we read the number of floats to load
	char* cursor = (char*)&contents[0];
	cursor += offset;
	GLuint floatCount;

	char* target = (char*)&floatCount;

	for (size_t i = 0; i < sizeof(GLuint); ++i) {
		target[i] = *cursor++;
	}
	offset += sizeof(GLuint) + floatCount * sizeof(GLfloat);
	vector.reserve(floatCount);

	//now read each float out
	for (size_t i = 0; i < floatCount; ++i) {

		GLfloat readFloat;

		target = (char*)&readFloat;

		for (size_t j = 0; j < sizeof(GLfloat); ++j) {
			target[j] = *cursor++;
		}

		vector.push_back(readFloat);
	}

	return offset;
}