#ifndef QUEEN_H
#define QUEEN_H

#include"ChessPiece.h"

/*
	This class represents a queen on the board
*/
class Queen : public ChessPiece {
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
	Queen(const ObjBinFile& file, const GLboolean color, const GLfloat boardWidth, const GLfloat boardHeight);

	/*
		Returns a constant specifying the calling class
	*/
	virtual GLuint getClass() const;

	/*
		The queen has no extra state data
	*/
	virtual std::vector<GLint> getClassData() const;
protected:

	/*
		@see ChessPiece::getValidMoves()
	*/
	virtual std::vector<Move> getValidMoves(const std::vector<ChessPiece*>& otherPieces, const GLboolean canEnterCheck);
};


#endif