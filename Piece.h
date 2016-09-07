#ifndef PIECE_H
#define PIECE_H

#include"GlObject.h"
#include"BoundingBox.h"
#include"ObjBinFile.h"

/*
	This class represents a piece in a game, it added collision detection with rays, and a rectangular bounding box
*/
class Piece : public GlObject {
public:
	/*
		Changes the settings on the bounding box
		@param lowerBound
			The bottom, left, back coordinate of the bounding box
		@param upperBound
			The top, right, front coordinate of the bounding box
	*/
	void setBoundingBox(const glm::vec3& lowerBound, const glm::vec3& upperBound);

	/*
		Checks if a ray intersects with this object, provided the distance to collision if it does
		@param ray
			The ray to check collision with
		@param distance
			The distance to intersection for reading
		@return true if the ray collides, false otherwise
	*/
	bool cursorIntersect(Ray ray, GLfloat& distance) const;

	/*
		Overriding to pass light data
	*/
	virtual void passUniforms() const;

	/*
		Links the light vector with the object, the object should not delete this vector
		@param lights
			A pointer to the vector of lights currently rendered
	*/
	void setLights(std::vector<glm::vec3>* lights);

protected:
	
	/*
		An axis aligned box used for collision checking
	*/
	BoundingBox box;

	/*
		A pointer to the set of lights on the board
	*/
	std::vector<glm::vec3>* pointLights;
};

#endif