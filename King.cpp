#include"King.h"

King::King(const ObjBinFile& file, const GLboolean color, const GLfloat boardWidth, const GLfloat boardHeight)
	: ChessPiece(file, color, boardWidth, boardHeight), hasMoved(GL_FALSE) {
	this->setBoundingBox(glm::vec3(-0.6f, -1.761832f, -0.6f), glm::vec3(0.6f, 1.283430f, 0.6f));
	this->scale(0.25f);
}

GLuint King::getClass() const { return KING; }

std::vector<GLint> King::getClassData() const {
	std::vector<GLint> data;
	return data;
}

std::vector<ChessPiece*> King::makeMove(const std::vector<ChessPiece*>& otherPieces, const Move& move) {
	//we want to check for a castle, the first easy requirement is a size 2 displacment on the x axis
	GLint dx = move.x - this->chessPosition.x;

	if (abs(dx) == 2) {
		//so we know that were castleing,
		//we need to find the rook in the direction were moving towards and move it to x - direction

		//first, which way are we going?
		GLint direction = abs(dx) / (dx);

		//now grab the rooks
		std::vector<ChessPiece*> rooks = this->fetchRooks(otherPieces);

		//now we need to find the closest rook in {direction}
		ChessPiece* theChosenRook = NULL;
		GLint distance = 100000000;
		for (size_t i = 0; i < rooks.size(); i++) {
			Move rookPos;
			rooks[i]->getLastChessPosition(rookPos);

			//if the distance towards the rook is shorter than any so far make it the chosen one
			if ((this->chessPosition.x - rookPos.x) * direction < distance) {
				theChosenRook = rooks[i];
				distance = (this->chessPosition.x - rookPos.x) * direction;
			}
		}

		if (theChosenRook != NULL) {
			theChosenRook->makeMove(otherPieces, Move(this->chessPosition.x + direction, this->chessPosition.z));
		}
	}
	return ChessPiece::makeMove(otherPieces, move);
}

std::vector<ChessPiece*> King::fetchRooks(const std::vector<ChessPiece*>& otherPieces) const {
	//now grab all the rooks of the same color as us that havent moved
	std::vector<ChessPiece*> rooks;
	for (size_t piece = 0; piece < otherPieces.size(); piece++) {
		if (otherPieces[piece]->getClass() == ROOK && otherPieces[piece]->getColor() == this->getColor()) {
			if (!otherPieces[piece]->getClassData()[0]) {
				rooks.push_back(otherPieces[piece]);
			}
		}
	}

	return rooks;
}

void King::onMoveCallback() {
	this->hasMoved = GL_TRUE;
}

std::vector<Move> King::getValidMoves(const std::vector<ChessPiece*>& otherPieces, const GLboolean canEnterCheck) {
	/*
	Kings can move in any direction onto opponent pieces or blank spaces, but only 1 place

	Kings cannot move into check

	Kings can castle, which means
	if the kings hasn't moved in the game
	and there is a straight path to a rook
	and said rook hasn't moved in the game
	then the king may move 2 squares towards the rook, and the rook  will be on the other side of the king
	if the kings passes through a zone in check during this move, the move is not allowed
	*/

	ChessBoard board = this->getBoardState(otherPieces);


	std::vector<Move> moves;

	std::vector<Move> partialMoves;

	//iterate over each direction around the queen
	for (GLint dx = -1; dx <= 1; dx++) {
		for (GLint dz = -1; dz <= 1; dz++) {
			//grab the pieces for this direction UNLESS the direction is 0, 0, which is not a direction
			if (dx != 0 || dz != 0) {
				partialMoves = this->checkMove(board, Move(dx, dz));
				moves.insert(moves.end(), partialMoves.begin(), partialMoves.end());
			}
		}
	}

	partialMoves = this->checkCastles(board, otherPieces, canEnterCheck);
	moves.insert(moves.end(), partialMoves.begin(), partialMoves.end());

	//if they are not allowed to move into check, filter out moves that put them in check
	if (!canEnterCheck) {
		this->filterByCheck(otherPieces, moves);
	}

	return moves;
}

std::vector<Move> King::checkMove(const ChessBoard& board, const Move& dMove) const {
	Move toCheck = this->chessPosition + dMove;

	GLint otherColor = this->getColor() == WHITE ? BLACK_PIECE : WHITE_PIECE;

	std::vector<Move> moves;

	if (toCheck.onBoard() && (board.at(toCheck) == otherColor || board.at(toCheck) == NONE)) {
		moves.push_back(toCheck);
	}

	return moves;
}

std::vector<Move> King::checkCastles(const ChessBoard& board, const std::vector<ChessPiece*>& otherPieces, const GLboolean canEnterCheck) const {
	std::vector<Move> moves;

	//first make sure we haven't moved
	if (this->hasMoved) {
		return moves;
	}

	//now grab all the rooks of the same color as us that havent moved
	std::vector<ChessPiece*> rooks = this->fetchRooks(otherPieces);

	//if no such rooks existed, return
	if (rooks.size() == 0) {
		return moves;
	}

	//lets grab the threat board before we start iterating on rooks,
	//we can't castle into check, 
	ChessBoard threatBoard;
	if (!canEnterCheck) {
		threatBoard = ChessPiece::getThreatBoard(!this->getColor(), otherPieces);
	}
	else {
		//on 2nd order checks, we essentially disable castling
		//because castling never captures a piece, therefore doesn't need to be considered when calculating threats
		for (size_t i = 0; i < 8; i++) {
			for (size_t j = 0; j < 8; j++) {
				threatBoard[i][j] = THREAT;
			}
		}
	}


	//now for each rook, we check that the path to it is clear, and cut it if its not
	for (size_t rook = 0; rook < rooks.size(); rook++) {
		//grab the rooks position
		Move rookPos;
		rooks[rook]->getLastChessPosition(rookPos);
		//if the z axis don't line up the question doesn't really make sense, so cut it and continue
		if (rookPos.z != this->chessPosition.z) {
			rooks.erase(rooks.begin() + rook);
			rook--;
			continue;
		}
		//if they are on the same x position thats also a problem, well get divide by 0 errors
		if (rookPos.x == this->chessPosition.x) {
			rooks.erase(rooks.begin() + rook);
			rook--;
			continue;
		}

		GLint dx = rookPos.x - this->chessPosition.x;
		//now iterate from the king to the rook and check that its all empty
		GLint direction = abs(dx) / (dx);
		GLboolean pathEmpty = GL_TRUE;

		for (GLint x = this->chessPosition.x + direction; x != rookPos.x; x += direction) {
			if (board[x][rookPos.z] != NONE) {
				pathEmpty = GL_FALSE;
				break;
			}
		}
		//if there was something in the path, cut it and move on
		if (!pathEmpty) {
			rooks.erase(rooks.begin() + rook);
			rook--;
			continue;
		}

		//the last check is that we can't move through check during the castle, so for each rook we
		//check 2 squares toward the rook for threats
		if (!Move(this->chessPosition.x + direction, rookPos.z).onBoard() || !Move(this->chessPosition.x + direction, rookPos.z).onBoard()) {
			rooks.erase(rooks.begin() + rook);
			rook--;
			continue;
		}

		//we can't be in or move throught check
		if (threatBoard[this->chessPosition.x][rookPos.z] == THREAT || 
			threatBoard[this->chessPosition.x + direction][rookPos.z] == THREAT ||
			threatBoard[this->chessPosition.x + 2 * direction][rookPos.z] == THREAT) {
			rooks.erase(rooks.begin() + rook);
			rook--;
			continue;
		}

		//any rooks left here are valid castling targets,
		//we just need to add the '2 moves toward the rook' move to the list of moves
		moves.push_back(Move(this->chessPosition.x + 2 * direction, rookPos.z));
	}

	return moves;
}