#include"BoundingBox.h"

bool BoundingBox::getIntersection(const glm::mat4x4& Model, const Ray& ray, GLfloat& collision, const GLfloat scale) const {

	//these two are reset every plane
	GLfloat tMin = 0.0f; //the closest possible collision
	GLfloat tMax = 100000.0f; //the farthest collision allowed

							  //for a collision, the ray must intersect every plane, or pass between them, this function checks both
	if (!this->planeIntersection(XPLANE, Model, ray, tMin, tMax, scale)) {
		return false;
	}

	if (!this->planeIntersection(YPLANE, Model, ray, tMin, tMax, scale)) {
		return false;
	}

	if (!this->planeIntersection(ZPLANE, Model, ray, tMin, tMax, scale)) {
		return false;
	}
	//assign the collision distance if there was a collision on all planes
	collision = tMin;
	return true;
}

bool BoundingBox::planeIntersection(GLuint plane, const glm::mat4x4& Model, const Ray& ray, GLfloat& tMin, GLfloat& tMax, const GLfloat scale) const {
	//the coordinate of (0, 0, 0) in the box in worldspace
	glm::vec3 boxPos(Model[3].x, Model[3].y, Model[3].z);

	glm::vec3 axis(Model[plane].x, Model[plane].y, Model[plane].z);

	//vector pointing 
	glm::vec3 delta = boxPos - ray.origin;


	//for some reason using scale squared makes all the scaling work ? 
	glm::vec3 minBoundS = glm::vec3(scale) * glm::vec3(scale) * this->minBound;
	glm::vec3 maxBoundS = glm::vec3(scale) * glm::vec3(scale) * this->maxBound;


	//I have no idea what this variable is
	GLfloat e = glm::dot(axis, delta);
	//cos(angle between direction vector and axis)
	//i.e if they are perpidicular, cos(pi/2) = 0
	GLfloat f = glm::dot(ray.direction, axis);

	//if f is too small, the direction vector is parelled to the planes, so we wont have an intersection on this plane
	if (fabs(f) > 0.0001f) {
		//intersection with close plane
		GLfloat t1 = 0.0f;
		//intersection with far plane
		GLfloat t2 = 0.0f;

		switch (plane) {
		case 0:
			t1 = (e + minBoundS.x) / f; // Intersection with the "left" plane
			t2 = (e + maxBoundS.x) / f; // Intersection with the "right" plane
			break;
		case 1:
			t1 = (e + minBoundS.y) / f; // Intersection with the "left" plane
			t2 = (e + maxBoundS.y) / f; // Intersection with the "right" plane
			break;
		case 2:
			t1 = (e + minBoundS.z) / f; // Intersection with the "left" plane
			t2 = (e + maxBoundS.z) / f; // Intersection with the "right" plane
			break;
		}

		// We want t1 to represent the nearest intersection, 
		// so if it's not the case, invert t1 and t2
		if (t1 > t2) {
			GLfloat w = t1; t1 = t2; t2 = w; // swap t1 and t2
		}

		// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
		if (t2 < tMax) {
			tMax = t2;
		}

		// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
		if (t1 > tMin) {
			tMin = t1;
		}


		// And here's the trick :
		// If "far" is closer than "near", then there is NO intersection.
		// See the images in the tutorials for the visual explanation.
		if (tMax < tMin) {
			return false;
		}
	}
	//else ray nearly parellel with plane, just check if its between the two planes
	else
	{
		switch (plane) {
		case 0:
			if (-e + minBoundS.x > 0.0f || -e + maxBoundS.x < 0.0f) {
				return false;
			}
			break;
		case 1:
			if (-e + minBoundS.y > 0.0f || -e + maxBoundS.y < 0.0f) {
				return false;
			}
			break;
		case 2:
			if (-e + minBoundS.z > 0.0f || -e + maxBoundS.z < 0.0f) {
				return false;
			}
			break;
		}

	}

	return true;
}