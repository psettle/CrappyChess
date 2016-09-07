#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include"common.h"
#include"Move.h"

//these constants are used in building the board
#define NONE 0
#define BLACK_PIECE 1
#define WHITE_PIECE 2

//these are constants used in determining the next move
#define PAWN_BLACK_PIECE_CAN_CAPTURE_EN_PASSENT 3
#define PAWN_WHITE_PIECE_CAN_CAPTURE_EN_PASSENT 4

/*
	This class is used to represent a board of chess data
	the constants above can be assigned to cells 
	to save data
*/
class ChessBoard : public std::vector<std::vector<GLint>> {
public:
	/*
		Creates an empty 8x8 board
	*/
	ChessBoard();

	/*
		Access a grid square by a move
		(with a modifyable return)
		@param move
			The move to access with
		@return 
			The grid value at the provided position
	*/
	GLint& at(const Move& move);
	
	/*
		Access a grid square by a move
		(with a constant return)
		@param move
			The move to access with
		@return
			The grid value at the provided position
	*/
	GLint at(const Move& move) const;

};

#endif
