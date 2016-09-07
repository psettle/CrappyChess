#ifndef BOARD_H
#define BOARD_H

#include"Piece.h"

/*
	This class represents the game board
*/
class Board : public Piece {
public:
	/*
		Loads the board files and initializes all members
	*/
	Board();

	/*
		Returns the height in worldspace of the top of the board
	*/
	GLfloat getTopHeight() const;

	/*
		Returns the width of this board
	*/
	GLfloat getWidth() const;
protected:
	/*
		This class just passes its material to the shader
	*/
	virtual void passUniforms() const;
};
#endif