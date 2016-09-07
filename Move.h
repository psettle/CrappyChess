#ifndef MOVE_H
#define MOVE_H

#include"common.h"

/*
	This class represents a move or a position on the chess board
*/
class Move {
public:
	/*
		Creates an empty move (x = 0, z = 0)
	*/
	Move();

	/*
		Creates a move at x, z
		@param x
			The x position for the move
		@param z
			The z position for the move
	*/
	Move(GLint x, GLint z);

	/*
		Checks if this move is on the board
		@return
			true if this move is on the board, false otherwise
	*/
	GLboolean onBoard() const;

	/*
		Checks if the two moves match by x and z coordinates
		@return 
			GL_TRUE if the move match,
			GL_FALSE if the moves don't match

	*/
	GLboolean operator==(const Move& RHS) const;

	/*
		Adds the RHS x and z coordinates with this objects x and z and returns
		@return 
			The summed move
	*/
	Move operator+(const Move& RHS) const;

	/*
		Adds another moves coordinates into this one
		@return
			A reference to this object for statements like x == y += z;
	*/
	Move& operator+=(const Move& RHS);

	/*
		The x coordinate of the move
	*/
	GLint x;

	/*
		The z coordinate of the move
	*/
	GLint z;
};

#endif