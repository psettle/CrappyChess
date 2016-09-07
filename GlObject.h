#ifndef GL_OBJECT_H
#define GL_OBJECT_H

#include"GlCamera.h"
#include"shader.h"
#include"texture.h"
#include"common.h"

/*
	This is the base class for objects withing the system
*/
class GlObject {
public:
	/*
		Safely initializes members
	*/
	GlObject();

	/*
		Adds vertex coordinates to the object, every 9 numbers forms a triangle (xyz)(xyz)(xyz)
		@param floats
			The list of vertex positions to add
	*/
	void addVertices(const std::vector<GLfloat>& floats);

	/*
		Adds UV data to the object, every 6 numbers go with one of the triangles added in 
		addVertices (uv)(uv)(uv)
		@param UVs
			The list of UVs to add
	*/
	void addUVs(const std::vector<GLfloat>& UVs);

	/*
		Adds RGB colors to the object, every 9 numbers are the colors for 1 triangles added in
		addVertices
		@param Colors
			The list of colors to add
	*/
	void addColors(const std::vector<GLfloat>& Colors);

	/*
		Adds the normal vectors to each coordinate added in addVertices
		(xyz)(xyz)(xyz)
	*/
	void addNormals(const std::vector <GLfloat>& Normals);

	/*
		Adds the shader program to this object
		@param vertexShader
			The filename of the vertexShader glsl file
		@param fragmentShader
			The filename of the fragmentShader glsl file
	*/
	void setShader(const std::string& vertexShader, const std::string& fragmentShader);

	/*
		Sets the shader for the object from an existing shader
		@param shader
			The shader to set
	*/
	void setShader(Shader* shader);

	/*
		Adds a pointer to the camera this object will be rendered through
		@param camera
			The camera to render the object through
	*/
	void setCamera(GlCamera* camera);

	/*
		Sets the texture file to be used on the object
		@param fileName
			The filename of the image file used for the texture
	*/
	void setTexture(const std::string& fileName);

	/*
		Sets the texture for an object from an existing texture
		@param texture
			The texture to set
	*/
	void setTexture(Texture* texture);

	/*
		Initializes the object, should be called after assigning the vertices, uvs, normals, shaders and textures
	*/
	virtual void initialize();

	/*
		Draw this object to the buffer, should be called from within GlSystem::frame()
	*/
	virtual void draw() const;

	/*
		Passes data to the glsl shaders, this is called within GlObject::draw()
		this can be overwritten to pass extra data
	*/
	virtual void passUniforms() const;

	/*
		Rotates this object in space
		@param angle
			The angle (in radian) to rotate the object
		@param axis
			The direction vector to rotate around
	*/
	void rotate(const GLfloat angle, const glm::vec3& axis);

	/*
		Translates the object is space by direction
		@param direction
			The vector to translate the object along
	*/
	void translate(const glm::vec3& direction);

	/*
		Resizes the object by the provided facter
		@param factor
			The amount to factor by , 2.0 is double size
	*/
	void scale(const GLfloat factor);

	/*
		Moves the object to the provided position
		@param position
			The new position for the object
	*/
	void position(const glm::vec3& position);

	/*
		Get the objects current position
		@return
			The objects current position
	*/
	glm::vec3 getPosition() const;

	/*
		Gets the current scaling factor for this object
		@return
			The scaling factor for the object
	*/
	GLfloat getScale() const;

	/*
		Reverts a position is world space to one in model space for this object
		@param worldVector
			The coordinates in world space to convert
		@return 
			The coordinates relative to the model
	*/
	glm::vec3 worldToModel(const glm::vec3& worldVector) const;

	/*
		Converts a coordinate relative to the model into worldspace
		@param modelVector
			The coordinates realitve to the model
		@return
			The coordinates in world space
	*/
	glm::vec3 modelToWorld(const glm::vec3& modelVector) const;

	/*
		Gets the model matrix for this object
	*/
	glm::mat4 getModel() const;


	/*
		Releases system resources
	*/
	virtual ~GlObject();
protected:
	/*
		Array of vertex data, seperated into groups of 9 for each triangle
	*/
	std::vector<GLfloat> vertices;

	/*
		Array of color data,seperated into groups of 9 for each triangle
	*/
	std::vector<GLfloat> colors;

	/*
		Array of color data,seperated into groups of 6 for each triangle
	*/
	std::vector<GLfloat> uvs;

	/*
	Array of normal data,seperated into groups of 9 for each triangle
	*/
	std::vector<GLfloat> normals;

	/*
		The filename of the vertex shader being used
	*/
	std::string vertexShader;

	/*
		The filename of the fragment shader being used
	*/
	std::string fragmentShader;

	/*
		the filename of the image being used as texture
	*/
	std::string textureFile;

	/*
		The vertex array object this object is bound to
	*/
	GLuint VAO;

	/*
		A list of buffers that need to be cleared when the object is destroyed
	*/
	std::vector<GLuint> buffers;

	/*
		The shader used for this object
	*/
	Shader* shader;

	/*
		The camera this object is viewed through
	*/
	GlCamera* camera;

	/*
		The texture for this object
	*/
	Texture* texture;

	/*
		The Model array, mapping the vertexes into world space (exludes scale and rotation)
	*/
	glm::mat4 Model;

	/*
		The rotation matrix, seperate of the model matrix
	*/
	glm::mat4 rotation;

	/*
		The scale this object is rendered at
	*/
	GLfloat current_scale;

	/*
		The positions of this object in world space
	*/
	glm::vec3 current_position;
};



#endif