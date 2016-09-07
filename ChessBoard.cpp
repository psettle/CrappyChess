#include"ChessBoard.h"

ChessBoard::ChessBoard() {
	//iterate over every column (x axis)
	for (GLuint x = 0; x < 8; x++) {
		//iterate over every row value (z axis)
		this->push_back(std::vector<GLint>());
		for (GLuint z = 0; z < 8; z++) {
			(*this)[x].push_back(NONE);
		}
	}
}

GLint& ChessBoard::at(const Move& move) {
	return (*this)[move.x][move.z];
}

GLint ChessBoard::at(const Move& move) const {
	return (*this)[move.x][move.z];
}