#ifndef OBJ_BIN_FILE_H
#define OBJ_BIN_FILE_H

#include"ObjFile.h"

/*
	This is a class for both converting .obj files into a easily readable binary format, and loading that binary format

	The extension for these files is .objbin, but it doesn't actually matter
	The format of the file is 3 lists of floats, each list of floats in prefixed by an integer type saying how many floats follow.
	the lists represent, vectors, uvs and normals, repectively

	Common sizes are 4 byte unsigned integers and 4 byte floats,
	so for example, the file might look like
	0x00 0x00 0x00 0x01 0x00 0x00 x00 0x00 0x00 0x00 0x00 0x02 0x00 0x00 x00 0x00 0x00 0x00 x00 0x00 0x00 0x00 0x00 0x00
	[ #float vertexes ] [the float       ] [#float uvs       ] [1st float       ] [2nd float       ] [#float normals   ]

	These files are platform dependent, so as part of the installation process for a program that will use these files,
	all the .obj files must be 'compiled' through this class to adjust for different data sizes and endian properties
*/
class ObjBinFile {
public:
	/*
		This contructor creates an empty file
	*/
	ObjBinFile();

	/*
		This constructor loads a .objbin file into arrays
	*/
	ObjBinFile(const std::string& fileName);

	/*
		Loads the floats out of contents into vector, starting at offset
		@param vector
			The vector to populate with floats
		@param contents
			The contents to fill the vector with
		@param offset
			The index of the char to start reading at
		@return 
			The next index after the vector is read (offset for the next vector)
	*/
	GLuint loadVector(std::vector<GLfloat>& vector, const std::vector<char>& contents, GLuint offset);

	/*
		This initializes the object from a .obj file
		@param fileName
			The .obj file to load
	*/
	void loadObjFile(const std::string& fileName);

	/*
		Saves the contents of the file to a .objbin file
		@param fileName
			The name of the .objbin file to create
	*/
	void saveObjBinFile(const std::string& fileName) const;

	/*
		Writes vector into contents
		@param contents
			The vector to write the binary into
		@param vector
			The vector to print into content
	*/
	void printVector(std::vector<char>& contents, const std::vector<GLfloat>& vector) const;

	/*
		Gets the currently loaded vertexes
	*/
	std::vector<GLfloat> getVertexes() const;

	/*
		Gets the currently loaded normals
	*/
	std::vector<GLfloat> getNormals() const;

	/*
		Gets the currently loaded uvs
	*/
	std::vector<GLfloat> getUVs() const;
private:
	/*
		The vertexes from the file
	*/
	std::vector<GLfloat> vertexes;

	/*
		The normals from the file
	*/
	std::vector<GLfloat> normals;

	/*
		The uvs from the file
	*/
	std::vector<GLfloat> uvs;
};

#endif