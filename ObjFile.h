#ifndef OBJ_FILE_H
#define OBJ_FILE_H

#include"file_h.h"
#include<exception>
#include"common.h"

/*
	This class reads .obj files and retrieves the uv, vertex and normal data
*/
class ObjFile {
public:
	/*
		Initialzies the object data from the provided file
		@param fileName
			The name of the file to load and parse
	*/
	ObjFile(const std::string& fileName);

	/*
		Returns a list of floats, where every 9 represent 3 vertexes (xyz, xyz, xyz)
	*/
	std::vector<GLfloat> getVertices() const;
	
	/*
		Returns a list of floats where every 9 represent 3 normals (xyz, xyz, xyz)
	*/
	std::vector<GLfloat> getNormals() const;

	/*
		Returns a list of floats where every 6 represent 3 uvs (uv, uv, uv)
	*/
	std::vector<GLfloat> getUVs() const;
private:
	/*
		Reads and parses a single line of the file
		@param line
			The line to read and parse
	*/
	void readLine(const std::string& line);

	/*
		Reads and parses a vertex line of the file
		@param line
			The line to read and parse
	*/
	void readVertex(const std::string& line);

	/*
		Reads and parses a uv line of the file
		@param line
			The line to read and parse
	*/
	void readUV(const std::string& line);

	/*
		Reads and parses a normal line of the file
		@param line
			The line to read and parse
	*/
	void readNormal(const std::string& line);

	/*
		Reads and parses a assocation line of the file
		@param line
			The line to read and parse
	*/
	void readTriangle(const std::string& line);

	/*
		And the file has been read into the object, this function
		groups triangles and fills the raw float data
		REQUIRES
			All of the indexes in this->triangles are valid i.e. have a defined association
	*/
	void groupVertexes();

	/*
		This is an array of read vertexes from the object file
	*/
	std::vector<glm::vec3> vertexes;

	/*
		This is an array of read uvs from the object file
		the 3rd slot is used for an index
	*/
	std::vector<glm::vec2> uvs;

	/*
		This is an array of read normals from the object file
		the 4th slot is used for an index
	*/
	std::vector<glm::vec3> normals;

	/*
		Every 9 numbers are a v/u/n, v/u/n, v/u/n mapping of indexes, defining 1 triangle
	*/
	std::vector<size_t> triangles;

	/*
		A list of floats, every 3 values is a vertex, and every 9 a triangle
	*/
	std::vector<GLfloat> rawVertexData;

	/*
	A list of floats, every 2 values is a uv coordinate, and every 6 a triangle
	*/
	std::vector<GLfloat> rawUVData;

	/*
		A list of floats, every 3 values is a normal, and every 9 a triangle
	*/
	std::vector<GLfloat> rawNormalData;
};

#endif OBJ_FILE_H