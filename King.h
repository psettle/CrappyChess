#ifndef KING_H
#define KING_H

#include"ChessPiece.h"

/*
	Represents a king on the board
*/
class King : public ChessPiece {
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
	King(const ObjBinFile& file, const GLboolean color, const GLfloat boardWidth, const GLfloat boardHeight);

	/*
		Returns a constant specifying the calling class
	*/
	virtual GLuint getClass() const;

	/*
		The king has no extra state data
	*/
	virtual std::vector<GLint> getClassData() const;

	/*
		@see ChessPiece::makeMove()

		overriding to move rook during castle
	*/
	virtual std::vector<ChessPiece*> makeMove(const std::vector<ChessPiece*>& otherPieces, const Move& move);

	/*
		note on virtualMove

		virtualMove does not need to be overwritten, because castles cannot be
		performed while your king is in check, so a castle is never a technique to 
		get out of check
	*/


	/*
		Fetches all rooks out of otherPieces that havent moved and are the same color as us
		@param otherPieces
			A list of pieces to search for rooks in
	*/
	std::vector<ChessPiece*> fetchRooks(const std::vector<ChessPiece*>& otherPieces) const;
private:
	/*
		Sets this object to have moved
	*/
	virtual void onMoveCallback();

	/*
		@see ChessPiece::getValidMoves()
	*/
	virtual std::vector<Move> getValidMoves(const std::vector<ChessPiece*>& otherPieces, const GLboolean canEnterCheck);

	/*
		Checks if this piece moving by dx and dz is a valid move
		@param board
			The board to check pieces on
		@param dx
			How far to move in the x direction
		@param dz
			How far to move in the z direction
		@return
			vector containing all the valid moves found
	*/
	std::vector<Move> checkMove(const ChessBoard& board, const Move& dMove) const;

	/*
		Checks if this king can castle with either rook, and returns the squares it could move to if it can
		@param board
			The board to check castles on
		@param canEntercheck
			Indicates if the king is allowed to castle into check,

	*/
	std::vector<Move> checkCastles(const ChessBoard& board, const std::vector<ChessPiece*>& otherPieces, const GLboolean canEnterCheck) const;

	/*
		Indicates if this piece has moved since the start of the game
	*/
	GLboolean hasMoved;
};

#endif