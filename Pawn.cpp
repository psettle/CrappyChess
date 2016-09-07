#include"Pawn.h"


Pawn::Pawn(const ObjBinFile& file, const GLboolean color, const GLfloat boardWidth, const GLfloat boardHeight)
	: ChessPiece(file, color, boardWidth, boardHeight), hasMoved(false)
{
	this->setBoundingBox(glm::vec3(-0.7f, -0.899605f, -0.7f), glm::vec3(0.7f, 0.857311, 0.7f));
	this->scale(0.32f);
}

void Pawn::onMoveCallback() {
	this->hasMoved = true;
}

GLuint Pawn::getClass() const { return PAWN; }

std::vector<GLint> Pawn::getClassData() const {
	std::vector<GLint> data;
	data.push_back(this->justMade2Step);
	return data;
}

std::vector<Move> Pawn::getForwardMoves(const ChessBoard& board) const {
	//by arbitrary decision, +z is forward for white pieces, and -z is forward for black pieces
	GLint forwardMod = this->color == WHITE ? 1 : -1;


	Move position = this->chessPosition;

	std::vector<Move> moves;

	//check the 1 forward move
	Move oneForward = Move(position.x, position.z + forwardMod);
	if (oneForward.onBoard() && board.at(oneForward) == NONE) {
		moves.push_back(oneForward);
		Move twoForward = Move(position.x, position.z + 2 * forwardMod);
		//if the 1 forward was allowed, and we haven't moved yet, check the 2 forward move
		if (!this->hasMoved) {
			if (twoForward.onBoard() && board.at(twoForward) == NONE) {
				moves.push_back(twoForward);
			}
		}
	}

	return moves;
}

std::vector<Move> Pawn::getDiagonalMoves(const ChessBoard& board) const {
	//decide if were going forwards or backwards
	GLint forwardMod = this->color == WHITE ? 1 : -1;
	std::vector<Move> moves;

	//get the colors were looking for
	GLboolean color = this->getColor();
	GLuint pieceType = color ? BLACK_PIECE : WHITE_PIECE;
	GLuint enPassType = color ? PAWN_BLACK_PIECE_CAN_CAPTURE_EN_PASSENT : PAWN_WHITE_PIECE_CAN_CAPTURE_EN_PASSENT;

	Move position = this->chessPosition;
	
	//check if we can capture on the diagonals

	//the "1 diagonal +x move"
	Move oneDiagpX = Move(position.x + 1, position.z + forwardMod);
	if (oneDiagpX.onBoard() && (board.at(oneDiagpX) == pieceType || board.at(oneDiagpX) == enPassType)) {
		moves.push_back(oneDiagpX);
	}

	//the "1 diagonal -x move"
	Move oneDiagmX = Move(position.x - 1, position.z + forwardMod);
	if (oneDiagmX.onBoard() && (board.at(oneDiagmX) == pieceType || board.at(oneDiagmX) == enPassType)) {
		moves.push_back(oneDiagmX);
	}

	//check the en passent moves

	//the +x enPassent
	//Plus X en Passent OK
	GLboolean pXenPassOK = true;
	Move plusX = Move(position.x + 1, position.z);
	//the criteria for en passent are
	//1 the diagonal spot is on the board
	//2 the diagonal spot is empty or contains a piece of the other color
	//3 the en passent target is on the board
	//4 the en passent target is a pawn that just made a size 2 jump
	pXenPassOK = pXenPassOK && oneDiagpX.onBoard();
	pXenPassOK = pXenPassOK && (board.at(oneDiagpX) == NONE || board.at(oneDiagpX) == pieceType);
	pXenPassOK = pXenPassOK && plusX.onBoard();
	pXenPassOK = pXenPassOK && board.at(plusX) == enPassType;
	if (pXenPassOK) {
		moves.push_back(oneDiagpX);
	}
	//the -x enPassent
	//minus X en Passent OK
	GLboolean mXenPassOK = true;
	Move minusX = Move(position.x - 1, position.z);
	mXenPassOK = mXenPassOK && oneDiagmX.onBoard();
	mXenPassOK = mXenPassOK && (board.at(oneDiagmX) == NONE || board.at(oneDiagmX) == pieceType);
	mXenPassOK = mXenPassOK && minusX.onBoard();
	mXenPassOK = mXenPassOK && board.at(minusX) == enPassType;
	if (mXenPassOK) {
		moves.push_back(oneDiagmX);
	}

	return moves;
}

std::vector<Move> Pawn::getValidMoves(const std::vector<ChessPiece*>& otherPieces, const GLboolean canEnterCheck) {
	/*
	Pawns can move forward 1 space, left or right 1 space if a piece of the opposite color is there
	or 2 spaces forward if they have never moved,
	also  "En Passent"
	if a pawn is next to a pawn that has just made a size two step, it can step behind that pawn to capture it
	*/

	std::vector<Move> moves;
	std::vector<Move> returnMoves;

	ChessBoard board = this->getBoardState(otherPieces);

	returnMoves = this->getForwardMoves(board);
	moves.insert(moves.begin(), returnMoves.begin(), returnMoves.end());

	returnMoves = this->getDiagonalMoves(board);
	moves.insert(moves.begin(), returnMoves.begin(), returnMoves.end());

	//if they are not allowed to move into check, filter out moves that put them in check
	if (!canEnterCheck) {
		this->filterByCheck(otherPieces, moves);
	}

	return moves;
}

ChessBoard Pawn::getBoardState(const std::vector<ChessPiece*>& otherPieces) const {
	// we start by assembling a board that contains all the information relevant to pawn movement

	//create a board
	ChessBoard board = ChessPiece::getBoardState(otherPieces);


	//for each piece, add it to the board state
	for (size_t i = 0; i < otherPieces.size(); i++) {

		Move otherPos;
		otherPieces[i]->getLastChessPosition(otherPos);
		//this handles the specific case of en passent
		if (otherPieces[i]->getClass() == PAWN) {
			std::vector<GLint> data = otherPieces[i]->getClassData();
			if (data[0] && otherPieces[i]->getColor()) {
				board.at(otherPos) = PAWN_WHITE_PIECE_CAN_CAPTURE_EN_PASSENT;
			} else if (data[0]) {
				board.at(otherPos) = PAWN_BLACK_PIECE_CAN_CAPTURE_EN_PASSENT;
			}
		}
	}

	return board;
}

std::vector<ChessPiece*> Pawn::makeMove(const std::vector<ChessPiece*>& otherPieces, const Move& move) {
	//when a pawn makes a move, there are 3 options, either
	//1 the pawn moved onto a black space
	//2 the pawn moved onto a filled space
	//3 the pawn performed an en passent
	//note that 2 and 3 can occur in the same move, or 1 and 3
	ChessBoard board = this->getBoardState(otherPieces);

	GLuint enPassColor = this->color ? PAWN_BLACK_PIECE_CAN_CAPTURE_EN_PASSENT : PAWN_WHITE_PIECE_CAN_CAPTURE_EN_PASSENT;

	std::vector<ChessPiece*> toDelete;

	//we need to check the place on the z axis we are from and the x axis we are going to
	//the neat trick here is that if we are going forward, that position is where we were, so theres no risk of causing 
	//a false capture
	Move enPassTarget = Move(move.x, this->chessPosition.z);

	if (board.at(enPassTarget) == enPassColor) {
		//so we know we will capture a en passent, we just need to find the element
		//by position and add it to the array
		for (size_t i = 0; i < otherPieces.size(); i++) {
			Move otherPos;
			otherPieces[i]->getLastChessPosition(otherPos);
			if (otherPos == enPassTarget && otherPieces[i] != this) {
				toDelete.push_back(otherPieces[i]);
			}
		}
	}

	//make the move
	std::vector<ChessPiece*> parentToDelete = ChessPiece::makeMove(otherPieces, move);

	//append the parent to delete to out en passent array
	toDelete.insert(toDelete.begin(), parentToDelete.begin(), parentToDelete.end());

	//return the pieces that were captured
	return toDelete;
}

std::vector<ChessPiece*> Pawn::virtualMove(const std::vector<ChessPiece*>& otherPieces, const Move& move) {
	ChessBoard board = this->getBoardState(otherPieces);

	GLuint enPassColor = this->color ? PAWN_BLACK_PIECE_CAN_CAPTURE_EN_PASSENT : PAWN_WHITE_PIECE_CAN_CAPTURE_EN_PASSENT;

	std::vector<ChessPiece*> toRestore;


	Move enPassTarget = Move(move.x, this->chessPosition.z);
	//we need to check the place on the z axis we are from and the x axis we are going to
	//the neat trick here is that if we are going forward, that position is where we were, so theres no risk of causing 
	//a false capture
	if (board.at(enPassTarget) == enPassColor) {
		//so we know we will capture a en passent, we just need to find the element
		//by position and add it to the array
		for (size_t i = 0; i < otherPieces.size(); i++) {
			Move otherPos;
			otherPieces[i]->getLastChessPosition(otherPos);
			if (otherPos == enPassTarget && otherPieces[i] != this) {
				otherPieces[i]->cachePositionAndSet(Move(0,0), GL_TRUE);
				toRestore.push_back(otherPieces[i]);
			}
		}
	}
	
	std::vector<ChessPiece*> parentToRestore = ChessPiece::virtualMove(otherPieces, move);

	//append the parent to restore to out en passent array
	toRestore.insert(toRestore.begin(), parentToRestore.begin(), parentToRestore.end());

	//return the pieces that were captured
	return toRestore;
}

void Pawn::setChessPosition(const Move& move) {
	//if the size of the step in the z direction is 2, set the flag true otherwise false,
	if (abs(move.z - this->chessPosition.z) == 2) {
		this->justMade2Step = true;
	} else {
		this->justMade2Step = false;
	}
	ChessPiece::setChessPosition(move);
}