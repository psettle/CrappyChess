#include"Rook.h"

Rook::Rook(const ObjBinFile& file, const GLboolean color, const GLfloat boardWidth, const GLfloat boardHeight)
	: ChessPiece(file, color, boardWidth, boardHeight), hasMoved(GL_FALSE)
{
	this->setBoundingBox(glm::vec3(-0.7f, -0.859215f, -0.7f), glm::vec3(0.7f, 1.144958f, 0.7f));
	this->scale(0.30f);
}

/*
Returns a constant specifying the calling class
*/
GLuint Rook::getClass() const { return ROOK; }

/*
The rook returns a single int specifying if it has moved since the beginning of the game
*/
std::vector<GLint> Rook::getClassData() const {
	std::vector<GLint> data;
	data.push_back(this->hasMoved);
	return data;
}

void Rook::onMoveCallback() {
	this->hasMoved = GL_TRUE;
}

std::vector<Move> Rook::getValidMoves(const std::vector<ChessPiece*>& otherPieces, const GLboolean canEnterCheck) {
	/*
	Rooks can move in straigt lines, stopping just before any pieces of the same color,
	or on any black pieces
	*/

	ChessBoard board = this->getBoardState(otherPieces);

	std::vector<Move> moves;

	std::vector<Move> partialMoves;

	//grab the moves for each straight direction and add them to the list
	partialMoves = this->checkPath(board, Move(-1, 0));
	moves.insert(moves.end(), partialMoves.begin(), partialMoves.end());

	partialMoves = this->checkPath(board, Move(1, 0));
	moves.insert(moves.end(), partialMoves.begin(), partialMoves.end());

	partialMoves = this->checkPath(board, Move(0, 1));
	moves.insert(moves.end(), partialMoves.begin(), partialMoves.end());

	partialMoves = this->checkPath(board, Move(0, -1));
	moves.insert(moves.end(), partialMoves.begin(), partialMoves.end());

	//if they are not allowed to move into check, filter out moves that put them in check
	if (!canEnterCheck) {
		this->filterByCheck(otherPieces, moves);
	}

	return moves;
}