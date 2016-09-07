#include"Bishop.h"

Bishop::Bishop(const ObjBinFile& file, const GLboolean color, const GLfloat boardWidth, const GLfloat boardHeight)
	: ChessPiece(file, color, boardWidth, boardHeight)
{
	this->setBoundingBox(glm::vec3(-0.5f, -1.179602, -0.5f), glm::vec3(0.5f, 1.379539f, 0.5f));
	this->scale(0.30f);
}

GLuint Bishop::getClass() const { return BISHOP; }

std::vector<GLint> Bishop::getClassData() const {
	std::vector<GLint> data;
	return data;
}

std::vector<Move> Bishop::getValidMoves(const std::vector<ChessPiece*>& otherPieces, const GLboolean canEnterCheck) {
	/*
	Bishops can move in diagonal lines, stopping just before any pieces of the same color,
	or on any black pieces
	*/

	ChessBoard board = this->getBoardState(otherPieces);

	std::vector<Move> moves;

	std::vector<Move> partialMoves;

	//grab the moves for each diagonal direction and add them to the list
	partialMoves = this->checkPath(board, Move(-1, -1));
	moves.insert(moves.end(), partialMoves.begin(), partialMoves.end());

	partialMoves = this->checkPath(board, Move(1, -1));
	moves.insert(moves.end(), partialMoves.begin(), partialMoves.end());

	partialMoves = this->checkPath(board, Move(-1, 1));
	moves.insert(moves.end(), partialMoves.begin(), partialMoves.end());

	partialMoves = this->checkPath(board, Move(1, 1));
	moves.insert(moves.end(), partialMoves.begin(), partialMoves.end());

	//if they are not allowed to move into check, filter out moves that put them in check
	if (!canEnterCheck) {
		this->filterByCheck(otherPieces, moves);
	}

	return moves;
}