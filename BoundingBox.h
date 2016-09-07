#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include"Ray.h"

#define XPLANE 0
#define YPLANE 1
#define ZPLANE 2

/*
	This class is used for physics calculations on a bounding box
	the box is assumed to be aligned with the axis in model space
*/
class BoundingBox {
public:
	/*
		The coordinate of 1 corner of the box
	*/
	glm::vec3 minBound;

	/*
		The corrdinate of the opposite corner of the box
	*/
	glm::vec3 maxBound;

	/*
		Checks if the worldspace ray intersects with this bounding box
		in worldspace, returns true if they collide, false otherwise
		if they collide, the closest collision will be placed in collision
		@param Model
			The model array used to move the associated object into world space
		@param ray
			The ray to check intersection on
		@param collision
			The distance to the closest collision, if there was a collision
	*/
	bool getIntersection(const glm::mat4x4& Model, const Ray& ray, GLfloat& collision, const GLfloat scale) const;
private:
	/*
		Checks for intersection on a single plain
		@param plane
			Constant specifying which plane to check, constants found above
		@param Model
			The model matrix used to move the surrounding box into world space
		@param ray
			The ray to check collision on
		@param tMin
			The closest an intersection is allowed to be, if the intersection is closer it doesn't count
			This value is filled in the a further one if a further collision is found
		@param tMax
			The furthest an intersection is allowed to be, if the intersetion is further, it doesn't count
			This value is filled in the a closer one if a closer collision is found
	*/
	bool planeIntersection(GLuint plane, const glm::mat4x4& Model, const Ray& ray, GLfloat& tMin, GLfloat& tMax, const GLfloat scale) const;
};

#endif