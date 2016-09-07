#include"ChessPiece.h"

Shader* ChessPiece::shader;
Texture* ChessPiece::white;
Texture* ChessPiece::black;

ChessPiece::ChessPiece(const ObjBinFile& file, const GLboolean color, const GLfloat boardWidth, const GLfloat boardHeight)
	: Piece(), color(color), boardWidth(boardWidth), boardHeight(boardHeight), chessPosition(-100, -100), virtualDeleted(GL_FALSE)
{

	this->addVertices(file.getVertexes());
	this->addNormals(file.getNormals());
	this->addUVs(file.getUVs());

	this->setShader(ChessPiece::shader);

	if (color) {
		this->setTexture(ChessPiece::white);
	}
	else {
		this->setTexture(ChessPiece::black);
	}

}

void ChessPiece::getLastChessPosition(Move& move) const {
	move = this->chessPosition;
}

void ChessPiece::revertPosition() {
	this->setChessPosition(this->grabbedPosition);
}

void ChessPiece::getGrabbed(const std::vector<ChessPiece*> otherPieces) {
	this->grabbedPosition = this->chessPosition;
	this->validMoves = std::vector<Move>();
	//we launch a thread here to calculate where we are allowed to go
	this->moveProccessingThread = std::thread(&ChessPiece::processGetGrabbed, this, otherPieces);
}

void ChessPiece::processGetGrabbed(const std::vector<ChessPiece*> otherPieces) {
	this->validMoves = this->getValidMoves(otherPieces, false);
}

std::vector<Move> ChessPiece::getValidMovesFromCache() {
	return this->validMoves;
}

GLboolean ChessPiece::getColor() const {
	return this->color;
}

std::vector<Move> ChessPiece::getThreats(const std::vector<ChessPiece*>& otherPieces) {
	if (!this->virtualDeleted) {
		return this->getValidMoves(otherPieces, true);
	} else {
		return std::vector<Move>();
	}
}

void ChessPiece::onBeforeValidMove() {
	this->moveProccessingThread.join();
}

void ChessPiece::onMoveCallback() {};

void ChessPiece::filterByCheck(const std::vector<ChessPiece*>& otherPieces, std::vector<Move>& moves) {
	//foreach move, we will set this pieces 'chess position' to the moves position, we will calculate the threat board

	//first find the king
	ChessPiece* king = NULL;
	for (size_t pos = 0; pos < otherPieces.size(); pos++) {
		if (otherPieces[pos]->getClass() == KING && otherPieces[pos]->getColor() == this->getColor()) {
			king = otherPieces[pos];
			break;
		}
	}
	//if there is no king check is not possible
	if (king == NULL) {
		return;
	}

	//then for each move, try it and see if it puts us in check
	for (size_t move = 0; move < moves.size(); move++) {
		std::vector<ChessPiece*> toRevert = this->virtualMove(otherPieces, moves[move]);

		ChessBoard board = getThreatBoard(!this->getColor(), otherPieces);

		//we calculate the kings position here, because the piece were moving might be the king itself
		Move kingPos;
		king->getLastChessPosition(kingPos);

		//check if that puts the king in check
		if (board.at(kingPos) == THREAT) {
			moves.erase(moves.begin() + move);
			move -= 1;
		}

		//revert all the pieces we moved
		for (size_t i = 0; i < toRevert.size(); i++) {
			toRevert[i]->restore();
		}
	}

}

std::vector<ChessPiece*> ChessPiece::virtualMove(const std::vector<ChessPiece*>& otherPieces, const Move& move) {
	//the default behavior for this method is to cache the pieces position,
	//move the piece to the requested location, tag the piece moved onto as deleted,
	//and return an array of the deleted piece and itself
	//pawn and king both have complex logic to consider the effects on the board of castling
	//and en passent capture
	this->cachePositionAndSet(move, GL_FALSE);

	std::vector<ChessPiece*> toRestore;
	toRestore.push_back(this);

	for (size_t i = 0; i < otherPieces.size(); i++) {
		if (otherPieces[i]->chessPosition == this->chessPosition && 
			otherPieces[i] != this) {

			otherPieces[i]->cachePositionAndSet(Move(), GL_TRUE);
		
			toRestore.push_back(otherPieces[i]);
		}
	}

	return toRestore;
}

void ChessPiece::restore() {
	//restore the position
	this->chessPosition = this->virtualCache;
	//set the piece back on the board
	this->virtualDeleted = GL_FALSE;
}

void cachePositionAndSet(const Move& move, GLboolean deleted) {
	this->virtualCache = this->chessPosition;
	this->chessPosition = move;
	this->virtualDeleted = deleted;
}

void ChessPiece::setChessPosition(const Move& move) {
	//the space between each row on the chess board
	GLfloat gridSpacing = this->boardWidth / 8;
	//the offset between to center of the board and the center of the next row
	GLfloat lineOffset = gridSpacing / 2;

	GLfloat xOffset = (move.x - 4) * gridSpacing + lineOffset;
	GLfloat yOffset = this->boardHeight + this->getBottomHeight();
	GLfloat zOffset = (move.z - 4) * gridSpacing + lineOffset;

	this->position(glm::vec3(xOffset, yOffset, zOffset));

	//and save our position
	this->chessPosition = move;
}

void ChessPiece::getChessPosition(Move& move) const {
	GLfloat gridSpacing = this->boardWidth / 8;
	GLfloat lineOffset = gridSpacing / 2;

	GLfloat xOffset = this->current_position.x;
	GLfloat zOffset = this->current_position.z;

	//these is just solving the equation for the xOffset in this->setChessPosition for x

	GLfloat floatXPos = (xOffset - lineOffset) / gridSpacing;
	GLfloat floatZPos = (zOffset - lineOffset) / gridSpacing;

	//now we need to round these value to the nearsest integer, they might be negative
	if (floatXPos < 0.0f) {
		floatXPos -= 0.5f;
	} else {
		floatXPos += 0.5f;
	}

	if (floatZPos < 0.0f) {
		floatZPos -= 0.5f;
	} else {
		floatZPos += 0.5f;
	}

	GLint xPos = (GLint)(floatXPos)+4;
	GLint zPos = (GLint)(floatZPos)+4;

	//assign the calculated positions
	move = Move(xPos, zPos);
}

GLboolean ChessPiece::validMove(const Move& move) const {
	//get the legal moves
	std::vector<Move> moves = this->validMoves;//this->getValidMoves(otherPieces, false);
												//iterate over each and see if our coordinates match
	for (size_t pos = 0; pos < moves.size(); pos++) {
		if (moves[pos] == move) {
			return true;
		}
	}
	return false;
}

std::vector<ChessPiece*> ChessPiece::makeMove(const std::vector<ChessPiece*>& otherPieces, const Move& move) {
	std::vector<ChessPiece*> toDelete;

	//so we know we will capture a place on the location we are going to, we need to find it
	//by position and add it to the array
	for (size_t i = 0; i < otherPieces.size(); i++) {
		Move piecePos;
		otherPieces[i]->getLastChessPosition(piecePos);
		if (piecePos == move && otherPieces[i] != this) {
			toDelete.push_back(otherPieces[i]);
		}
	}

	//move the piece into position
	this->setChessPosition(move);

	this->onMoveCallback();

	return toDelete;
}

ChessBoard ChessPiece::getBoardState(const std::vector<ChessPiece*>& otherPieces) const {
	//create a board
	ChessBoard board;

	//for each piece, add it to the board state
	for (size_t i = 0; i < otherPieces.size(); i++) {
		ChessPiece* otherPiece = otherPieces[i];

		//grab the otherpiece's location
		Move piecePos;
		otherPiece->getLastChessPosition(piecePos);

		if (otherPiece->getColor()) {
			board.at(piecePos) = WHITE_PIECE;
		} else {
			board.at(piecePos) = BLACK_PIECE;
		}
	}

	return board;
}

std::vector<Move> ChessPiece::checkPath(const ChessBoard& board, const Move& dMove) const {
	GLuint otherColor = this->getColor() == WHITE ? BLACK_PIECE : WHITE_PIECE;
	GLuint ourColor = this->getColor() == WHITE ? WHITE_PIECE : BLACK_PIECE;

	Move toCheck = this->chessPosition + dMove;

	std::vector<Move> moves;

	//while the piece is on the board, and is either over a piece of the other color or no piece
	while (toCheck.onBoard() && (board.at(toCheck) == otherColor || board.at(toCheck) == NONE)) {
		//add the moves to the array and move to the next square
		moves.push_back(toCheck);
		
		//break if the piece was of the other color, so we don't go through them
		if (board.at(toCheck) == otherColor) {
			break;
		}

		//increment the move
		toCheck += dMove;
	}

	return moves;
}

ChessBoard ChessPiece::getThreatBoard(const GLboolean color, const std::vector<ChessPiece*>& otherPieces) {
	std::vector<Move> moves;
	//get the threatend spaces
	for (size_t piece = 0; piece < otherPieces.size(); piece++) {
		if (otherPieces[piece]->getColor() == color) {
			std::vector<Move> otherMoves = otherPieces[piece]->getThreats(otherPieces);
			moves.insert(moves.end(), otherMoves.begin(), otherMoves.end());
		}
	}
	//initializes board
	ChessBoard board;

	//apply the spaces
	for (size_t move = 0; move < moves.size(); move++) {
		board.at(moves[move]) = THREAT;
	}

	//return the board
	return board;
}