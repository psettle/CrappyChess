#include"Move.h"

Move::Move() : x(0), z(0) {}

Move::Move(GLint x, GLint z) : x(x), z(z) {}

GLboolean Move::onBoard() const {
	if (this->x < 0 || this->x > 7 || this->z < 0 || this->z > 7) {
		return GL_FALSE;
	}
	return GL_TRUE;
}

GLboolean Move::operator==(const Move& RHS) const {
	if (this->x == RHS.x && this->z == RHS.z) {
		return GL_TRUE;
	}
	return GL_FALSE;
}

Move Move::operator+(const Move& RHS) const {
	Move toReturn = Move(this->x + RHS.x, this->z + RHS.z);

	return toReturn;
}

Move& Move::operator+=(const Move& RHS) {
	this->x += RHS.x;
	this->z += RHS.z;
	return *this;
}