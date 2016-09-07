#ifndef PAWN_H
#define PAWN_H

#include"ChessPiece.h"

/*
	This class represents a pawn on the board
*/
class Pawn : public ChessPiece {
public:
	/*
		Initializes the pawn info
		@param file
			The loaded .obj file the pawn will use as a model
		@param color
			either BLACK or WHITE
		@param boardWidth 
			the width of the board this pawn is placed on
		@param boardHeight
			The height of the board this pawn is placed on
	*/
	Pawn(const ObjBinFile& file, const GLboolean color, const GLfloat boardWidth, const GLfloat boardHeight);

	/*
		@see ChessPiece::setChessPosition
			Override to check when a size to step is made and set an internal flag
	*/
	virtual void setChessPosition(const Move& move);

	/*
		@see ChessPiece::getValidMoves
	*/
	virtual std::vector<Move> Pawn::getValidMoves(const std::vector<ChessPiece*>& otherPieces, const GLboolean canEnterCheck);

	/*
		@see ChessPiece::virtualMove
	*/
	virtual std::vector<ChessPiece*> virtualMove(const std::vector<ChessPiece*>& otherPieces, const Move& move);

	/*
		@see ChessPiece::onMoveCallback()
	*/
	virtual void onMoveCallback();

	/*
		Performs the requested move, and returns a vector of pointer for pieces that were captured during the move
		@param otherPieces
			A vector of all pieces on the board
		@param x
			The x coordinate to move the piece to
		@param z
			The z coordinate to move the piece to

	*/
	virtual std::vector<ChessPiece*> makeMove(const std::vector<ChessPiece*>& otherPieces, const Move& move);

	/*
		Returns a constant specifying the calling class
	*/
	virtual GLuint getClass() const;

	/*
		This classes data is commposed of a boolean
		data member that indicates if this pawn made a 2 step jump last move
	*/
	virtual std::vector<GLint> getClassData() const;

	/*
		Builds a 2d vector of constants defined above from the list of all pieces on the board
		this board is used to calculate moves
	*/
	virtual ChessBoard getBoardState(const std::vector<ChessPiece*>& otherPieces) const;

protected:

	/*
		Checks if a move forward by howMuch is legal
		@param board
			The built board were checking on
		@return 
			A vector of moves representing all the moves that can be performed forwards
	*/
	std::vector<Move> getForwardMoves(const ChessBoard& board) const;

	/*
		Checks if the diagonal moves are valid, this does consider en passent captures
		@param board
			The build board to check  captures on
		@return 
			The valid moves that were found
	*/
	std::vector<Move> getDiagonalMoves(const ChessBoard& board) const;
	
	/*
		Indicates if this piece has been moved since the game started
	*/
	GLboolean hasMoved;

	/*
		If true, this pawn has just made a size 2 step
	*/
	GLboolean justMade2Step;
};

#endif