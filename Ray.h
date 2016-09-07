#ifndef RAY_H
#define RAY_H

#include"common.h"

/*
	This struct represents a ray in space
*/
struct Ray {
	/*
		The origin of the ray
	*/
	glm::vec3 origin;

	/*
		The direction the ray propagates in
	*/
	glm::vec3 direction;
};

#endif