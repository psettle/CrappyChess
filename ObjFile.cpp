#include"ObjFile.h"

ObjFile::ObjFile(const std::string& fileName) {
	std::string contents = file_get_contents(fileName);

	std::vector<std::string> lines = explode('\n', contents);

	for (size_t line = 0; line < lines.size(); line++) {
		this->readLine(lines[line]);
	}

	this->groupVertexes();
}

std::vector<GLfloat> ObjFile::getVertices() const {
	return this->rawVertexData;
}

std::vector<GLfloat> ObjFile::getNormals() const {
	return this->rawNormalData;
}

std::vector<GLfloat> ObjFile::getUVs() const {
	return this->rawUVData;
}

void ObjFile::readLine(const std::string& line) {
	//special case, return for empty string
	if (line.length() == 0) {
		return;
	}
	//find the type of line being dealt with
	char firstCharacter = line[0];
	switch (firstCharacter) {
	case '#': //comments stary with #
		break;
	case 'v':
	{
		//special case, return for 1 character line
		if (line.length() == 1) {
			break;
		}
		char secondCharacter = line[1];
		if (secondCharacter == ' ')
		{
			this->readVertex(line);
		}
		else if (secondCharacter == 't')
		{
			this->readUV(line);
		}
		else if (secondCharacter == 'n')
		{
			this->readNormal(line);
		}
		break;
	}
	case 'f':
		this->readTriangle(line);
		break;
	}
}

void ObjFile::readVertex(const std::string& line) {
	std::vector<std::string> parts = explode(' ', line);
	//if there arent exactly 4 parts then the string was formatted wrong
	if (parts.size() != 4) {
		return;
	}

	glm::vec3 vertex(
		(GLfloat)atof(parts[1].c_str()),
		(GLfloat)atof(parts[2].c_str()),
		(GLfloat)atof(parts[3].c_str())
	);

	this->vertexes.push_back(vertex);
}

void ObjFile::readUV(const std::string& line) {
	std::vector<std::string> parts = explode(' ', line);
	//if there arent exactly 3 parts then the string was formatted wrong
	if (parts.size() != 3) {
		return;
	}

	glm::vec2 uv(
		(GLfloat)atof(parts[1].c_str()),
		1.0f - (GLfloat)atof(parts[2].c_str()) //open GL inverts y coordinates
	);

	this->uvs.push_back(uv);
}

void ObjFile::readNormal(const std::string& line) {
	std::vector<std::string> parts = explode(' ', line);
	//if there arent exactly 4 parts then the string was formatted wrong
	if (parts.size() != 4) {
		return;
	}

	glm::vec3 normal(
		(GLfloat)atof(parts[1].c_str()),
		(GLfloat)atof(parts[2].c_str()),
		(GLfloat)atof(parts[3].c_str())
	);

	this->normals.push_back(normal);
}

void ObjFile::readTriangle(const std::string& line) {
	//explode line on spaces first
	std::vector<std::string> parts = explode(' ', line);

	if (parts.size() != 4) {
		return;
	}

	for (size_t set = 1; set < 4; set++) {
		std::vector<std::string> numbers = explode('/', parts[set]);

		if (numbers.size() != 3) {
			return;
		}

		for (size_t number = 0; number < numbers.size(); number++) {
			this->triangles.push_back(atoi(numbers[number].c_str()));
		}
	}
}

void ObjFile::groupVertexes() {
	//iterate over every group of 3 and push the data onto the raw data arrays
	for (size_t index = 0; index < this->triangles.size(); index += 3) {
		size_t pos = index;

		glm::vec3 vertex = this->vertexes[this->triangles[pos] - 1]; //substract 1 because indexing starts at 1 for the obj files
		glm::vec2 uv = this->uvs[this->triangles[pos + 1] - 1];
		glm::vec3 normal = this->normals[this->triangles[pos + 2] - 1];

		this->rawVertexData.push_back(vertex.x);
		this->rawVertexData.push_back(vertex.y);
		this->rawVertexData.push_back(vertex.z);

		this->rawUVData.push_back(uv.x);
		this->rawUVData.push_back(uv.y);

		this->rawNormalData.push_back(normal.x);
		this->rawNormalData.push_back(normal.y);
		this->rawNormalData.push_back(normal.z);
	}
}