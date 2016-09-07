#include"Knight.h"

Knight::Knight(const ObjBinFile& file, const GLboolean color, const GLfloat boardWidth, const GLfloat boardHeight)
	: ChessPiece(file, color, boardWidth, boardHeight)
{
	this->setBoundingBox(glm::vec3(-0.7f, -1.0000f, -0.7f), glm::vec3(0.7f, 2.374836f, 0.7f));

	this->rotate(- PI / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	//knights don't have rotational symetry, so if its black we need to rotate it 180 degrees
	if (color == BLACK) {
		this->rotate(PI, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	this->scale(0.18f);
}

GLuint Knight::getClass() const { return KNIGHT; }

std::vector<GLint> Knight::getClassData() const {
	std::vector<GLint> data;
	return data;
}

std::vector<Move> Knight::getValidMoves(const std::vector<ChessPiece*>& otherPieces, const GLboolean canEnterCheck) {
	/*
	Knights can move in 'L' shapes, regardless of pieces in between
	*/

	ChessBoard board = this->getBoardState(otherPieces);

	std::vector<Move> moves;
	std::vector<Move> tempMoves;

	std::vector<Move> partialMoves;

	//a way to express knight moves is to compine +/- 1 with +/- 2, so long as x != z
	//i.e +1, +2   -1 +2   -1 -2 
	for (GLint dx = -2; dx <= 2; dx += 1) {
		for (GLint dz = -2; dz <= 2; dz += 1) {
			//well check if the move is valid if x != 0, z != 0 and z != x
			if (dx != 0 && dz != 0 && abs(dz) != abs(dx)) {
				tempMoves = this->checkHook(board, Move(dx, dz));
				moves.insert(moves.begin(), tempMoves.begin(), tempMoves.end());
			}
		}
	}

	//if they are not allowed to move into check, filter out moves that put them in check
	if (!canEnterCheck) {
		this->filterByCheck(otherPieces, moves);
	}

	return moves;
}

std::vector<Move> Knight::checkHook(const ChessBoard& board, const Move& dMove) const {
	GLint ourColor = this->color == WHITE ? WHITE_PIECE : BLACK_PIECE;

	Move target = this->chessPosition + dMove;

	std::vector<Move> moves;

	if (target.onBoard() && board.at(target) != ourColor) {
		moves.push_back(target);
	}

	return moves;
}