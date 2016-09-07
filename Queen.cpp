#include"Queen.h"

Queen::Queen(const ObjBinFile& file, const GLboolean color, const GLfloat boardWidth, const GLfloat boardHeight)
	: ChessPiece(file, color, boardWidth, boardHeight)
{
	this->setBoundingBox(glm::vec3(-0.6f, -1.761832f, -0.6f), glm::vec3(0.6f, 1.283430f, 0.6f));
	this->scale(0.25f);
}

GLuint Queen::getClass() const { return QUEEN; }

std::vector<GLint> Queen::getClassData() const {
	std::vector<GLint> data;
	return data;
}

std::vector<Move> Queen::getValidMoves(const std::vector<ChessPiece*>& otherPieces, const GLboolean canEnterCheck) {
	/*
	Queens can move in any direction, stopping just before any pieces of the same color,
	or on any black pieces
	*/

	ChessBoard board = this->getBoardState(otherPieces);


	std::vector<Move> moves;

	std::vector<Move> partialMoves;

	//iterate over each direction around the queen
	for (GLint dx = -1; dx <= 1; dx++) {
		for (GLint dz = -1; dz <= 1; dz++) {
			//grab the pieces for this direction UNLESS the direction is 0, 0, which is not a direction
			if (dx != 0 || dz != 0) {
				partialMoves = this->checkPath(board, Move(dx, dz));
				moves.insert(moves.end(), partialMoves.begin(), partialMoves.end());
			}
		}
	}

	//if they are not allowed to move into check, filter out moves that put them in check
	if (canEnterCheck == GL_FALSE) {
		this->filterByCheck(otherPieces, moves);
	}

	return moves;
}