#ifndef GL_CAMERA_H
#define GL_CAMERA_H

#include"Ray.h"
#include"common.h"

/*
	Deals with all perspective effects for rendering an object
*/
class GlCamera {
public:
	/*
	Safely initailizes members
	*/
	GlCamera(GLFWwindow* window = NULL);

	/*
		Does required processing for a frame,
		to pass data define a function an inherited classes to pass data, and access
		data in this definintion
	*/
	virtual void frame() {}

	/*
		Passes data to the class, what the data means
		it derived type dependant
	*/
	virtual void passData(const std::vector<GLint> data);

	/*
		'Looks at' a direction
	*/
	void newView(const glm::vec3& from, const glm::vec3& to, const glm::vec3& up);

	/*
		Creates a perspective for looking at a point
	*/
	void newPerspective(const float& fov, const size_t width, const size_t height, const float front, const float back);

	/*
		Applies the camera effect to an input transformation, returns the resulting transform
	*/
	glm::mat4 feedMatrix(const glm::mat4& source) const;

	/*
		Safely disposes of resourcess
	*/
	void shutdown();

	/*
		Reverts a ray to world space
		@param start
			The starting point of the ray in view space
		@param end
			The end point of the ray in  world space
	*/
	void rayToWorldspace(glm::vec4& start, glm::vec4& end) const;

	/*
		Gets this cameras position in worldspace
	*/
	glm::vec3 getPosition() const;

	/*
		Safely clears camera resources
	*/
	virtual ~GlCamera();
protected:
	/*
		The perspective matrix effect
	*/
	glm::mat4 Projection;

	/*
		The look at matrix effect
	*/
	glm::mat4 View;

	/*
		The sum camera transform, cached to increase speed
	*/
	glm::mat4 ResultCache;

	/*
		Window binding
	*/
	GLFWwindow* pWindowM;

	/*
		The position this camera is at
	*/
	glm::vec3 positionM;
};



#endif
