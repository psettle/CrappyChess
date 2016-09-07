#ifndef ROOK_H
#define ROOK_H

#include"ChessPiece.h"

/*
	This class represents a rook on the board
*/
class Rook : public ChessPiece {
public:
	/*
		Initializes members
		@param file
			The loaded .obj file the pawn will use as a model
		@param color
			either WHITE or BLACK, controls behavior of the piece and its texture
		@param boardWidth
			how wide the board is, this is used for positioning the piece on the grid squares
		@param boardHeight
			How tall the board is, or more accurately, how far the top of the board is from the x = 0 point on the y axis
	*/
	Rook(const ObjBinFile& file, const GLboolean color, const GLfloat boardWidth, const GLfloat boardHeight);

	/*
		Returns a constant specifying the calling class
	*/
	virtual GLuint getClass() const;

	/*
		The rook returns a single int specifying if it has moved since the beginning of the game
	*/
	virtual std::vector<GLint> getClassData() const;

	/*
		Sets this piece as having moved since the start of the game
	*/
	virtual void onMoveCallback();
protected:
	/*
		@see ChessPiece::getValidMoves()
	*/
	virtual std::vector<Move> getValidMoves(const std::vector<ChessPiece*>& otherPieces, const GLboolean canEnterCheck);

	/*
		Indicates if this piece has moved since the start of the game
	*/
	GLboolean hasMoved;
};

#endif