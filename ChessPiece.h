#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include"Piece.h"
#include"Move.h"
#include"ChessBoard.h"

//these are used for getting a object type during run time
#define PAWN 1
#define QUEEN 2
#define ROOK 3
#define BISHOP 4
#define KNIGHT 5
#define KING 6


//these constants are used when checking for check/checkmates
#define SAFE 0
#define THREAT 1

/*
	This class represents a chess piece on the board
	This class is an abstract base class for all types of pieces

	This class has the static members: shader, white, black
	these get initialized by ChessSystem::initialize(), and deleted by ~ChessSystem
	so any ChessPieces created must be created between those two calls
*/
class ChessPiece : public Piece{
public:
	/*
		Initializes members
		@param color
			either WHITE or BLACK, controls behavior of the piece and its texture
		@param boardWidth
			how wide the board is, this is used for positioning the piece on the grid squares
		@param boardHeight
			How tall the board is, or more accurately, how far the top of the board is from the x = 0 point on the y axis
	*/
	ChessPiece(const ObjBinFile& file, const GLboolean color, const GLfloat boardWidth, const GLfloat boardHeight);
	
	/*
		Sets the piece on the board an the provided position
		@param x
			The x position on the chess board (left to right), 0 is the leftmost, 7 is the rightmost
		@param z
			The z position on the chess board (front to back), 0 is the closest, 7 is the farthest
	*/
	virtual void setChessPosition(const Move& move);

	/*
		Gets the pieces current position from its position in world space
		@param x (container)
			The x position on the chess board (left to right), 0 is the leftmost, 7 is the rightmost
		@param z (container)
			The z position on the chess board (front to back), 0 is the closest, 7 is the farthest
	*/
	void getChessPosition(Move& move) const;

	/*
		Gets the pieces current position from the last place it was put on the board
		@param x (container)
			The x position on the chess board (left to right), 0 is the leftmost, 7 is the rightmost
		@param z (container)
			The z position on the chess board (front to back), 0 is the closest, 7 is the farthest
	*/
	void getLastChessPosition(Move& move) const;

	/*
		Reverts this piece to its location when getGrabbed() was called
	*/
	void revertPosition();

	/*
		Saves this pieces position and starts the moveProcessing thread
		@param otherPieces
			All the pieces in the system
	*/
	void getGrabbed(const std::vector<ChessPiece*> otherPieces);

	/*
		This is the callback for the moveProccessingThread, it stores the valid moves into the class's cache
		@param otherPieces
			All the pieces in the system
	*/
	void processGetGrabbed(const std::vector<ChessPiece*> otherPieces);

	/*
		Retreives the list of valid moves from the cache, these are only valid during the period in between 
		before and after the results will be invalid
		this->getGrabbed 
		and 
		this->makeMove
	*/
	std::vector<Move> getValidMovesFromCache();

	/*
		Returns this piece's color, one of WHITE or BLACK
	*/
	GLboolean getColor() const;

	/*
		Checks if the provided move is a valid move for this piece to make

		This function will only work after calling
		this->getGrabbed()
		then
		this->onBeforeValidMove

		@param otherPieces
			A vector of all the other pieces on the board
		@param x
			The x coordinate of the move to be checked
		@param z
			The z coordinate of the move to be checked
	*/
	GLboolean validMove(const Move& move) const;

	/*
		Causes this piece to perform a move, returns an array of pieces that need to be deleted after the move
		note that this function does not check that the move is legal, call validMove for that
		@param otherPieces
			The other pieces on the board, used to find the ones to delete
		@param x
			The x position to move to on the board
		@param z
			The z position to move to on the board
		@return 
			A vector of pieces to delete, there is actually 1 type of move that can capture 2 pieces
	*/
	virtual std::vector<ChessPiece*> makeMove(const std::vector<ChessPiece*>& otherPieces, const Move& move);

	/*
		Executes a "fake move" moving the piece to the position in its internal data
		and returning a list of all other pieces affected by the move,
		after calling this method, revertMove should be called on every returned piece
		@param x
			The x position to move to
		@param z
			The z position to move to
	*/
	virtual std::vector<ChessPiece*> virtualMove(const std::vector<ChessPiece*>& otherPieces, const Move& move);

	/*
		Restores this piece to its original state after a virtual move
	*/
	void restore();

	void cachePositionAndSet(const Move& move, GLboolean deleted) {
		this->virtualCache = this->chessPosition;
		this->chessPosition = move;
		this->virtualDeleted = deleted;
	}

	/*
		Gets the squares on the board that this piece threatens
	*/
	virtual std::vector<Move> getThreats(const std::vector<ChessPiece*>& otherPieces);

	/*
		Returns 1 of the contants above depending on what kind of piece it is
	*/
	virtual GLuint getClass() const = 0;

	/*
		Returns an array of integer data describing attributes of the class
		what each number means in this array is defined by derived types, 
		getClass can be used to figure out what kind of data is being dealt with
	*/
	virtual std::vector<GLint> getClassData() const = 0;

	/*
		Call after this->getGrabbed() and before this->ValidMove()
		This functions blocks until the move processing thread finished
		garaunteeing that the legal moves is correct before checking that a move is valid
	*/
	void onBeforeValidMove();

	/*
		The shader program used by all chesspieces
		by keeping it as a static member, it reduces unrequired compilation
	*/
	static Shader* shader;

	/*
		The texture object shared by all white pieces, by keeping it staic
		it reduces file loading
	*/
	static Texture* white;

	/*
		The texture object used by all black pieces, keeping it static reduces file loading
	*/
	static Texture* black;

	/*
		@return
			Returns the magnitude of the distance from the bottom of this piece to its origin
	*/
	GLfloat getBottomHeight() {
		return - this->box.minBound.y * this->current_scale;
	}
protected:

	virtual void passUniforms() const {
		Piece::passUniforms();
		GLuint progID = this->shader->getProgramID();
		//just pass the material here
		//the material for these pieces is all the same
		GLuint matAmbientLoc = glGetUniformLocation(progID, "material.ambient");
		GLuint matDiffuseLoc = glGetUniformLocation(progID, "material.diffuse");
		GLuint matSpecularLoc = glGetUniformLocation(progID, "material.specular");
		GLuint matShineLoc = glGetUniformLocation(progID, "material.shininess");

		glUniform3f(matAmbientLoc, 0.6f, 0.6f, 0.6f);
		glUniform3f(matDiffuseLoc, 0.2f, 0.2f, 0.2f);
		glUniform3f(matSpecularLoc, 0.1f, 0.1f, 0.1f);
		glUniform1f(matShineLoc, 1.0f);
	}
	/*
		Gets a board populated with SAFE or THREAT, where THREAT are the spaces on the board
		that are threatend by the provided color, i.e could be captured next move

		This array does ignore EN PASSENT threat, but thats okay because this is only used for kings,
		and kings cannot be captured with en passent

		@param color
			The threating color
		@param otherPieces
			All the pieces on the board
		@return 
			A board populated with THREAT or SAFE indicating which squares on the board
			are threatend by the provided color

	*/
	static ChessBoard getThreatBoard(const GLboolean color, const std::vector<ChessPiece*>& otherPieces);

	/*
		This function is called after an object is placed, this is where pieces can sort themselves out after moving
	*/
	virtual void onMoveCallback();

	/*
		Builds a 2d vector of constants defined above from the list of all pieces on the board
		this board is used to calculate moves. The default board just maps the colors of other pieces
		derived types may override this method if they require other state based information
		(such as pawns which require knowledge of a piece's last move to execute a en passent capture)
		@param otherPieces
			A vector of other piece to contruct the board state on
		@return
			the built board object
	*/
	virtual ChessBoard getBoardState(const std::vector<ChessPiece*>& otherPieces) const;

	/*
		Returns a list of of integers, where every pair represents the grid coordinates of a valid moves
		that a piece can make
		@param board
			The result of Board->toArray(), this is done to prevent circular dependencies
		@param canEnterCheck
			Indicates if the current user is allowed to make a move that would put them in check
			This occurs when considering 2nd order check, i.e. When making a move that would put you in check,
			you still can't make it if the only way the opponent can capture your king puts them in check
	*/
	virtual std::vector<Move> getValidMoves(const std::vector<ChessPiece*>& otherPieces, const GLboolean canEnterCheck) = 0;

	/*
		Filters the provided list of moves by removing any that would put the colors king under check
		@param otherPieces
			All the pieces on the board
		@param moves
			The initial (unfiltered) list of moves
	*/
	void filterByCheck(const std::vector<ChessPiece*>& otherPieces, std::vector<Move>& moves);

	/*
		Returns an array of valid moves in the provided dx and dy direction
		This is a pretty common requirement for chess pieces, rooks, bishops and queens all need to perform this calculation
		@param board
			The build board object to check move validity on
	*/
	std::vector<Move> checkPath(const ChessBoard& board, const Move& dMove) const;

	/*
		The color of this piece one of WHITE or BLACK
	*/
	GLboolean color;

	/*
		A save variable for tracking a piece's initial position when it gets grabbed
	*/
	Move grabbedPosition;

	/*
		The xPosition the piece was last set at
		this is stored to make calculating game state and possible moves faster
	*/
	Move chessPosition;

	/*
		The caches for this pieces actually position during check detection
	*/
	Move virtualCache;

	/*
		The world space width of the board this piece is on
	*/
	GLfloat boardWidth;

	/*
		The world space height of the top surface of the board this piece is on
	*/
	GLfloat boardHeight;

	/*
		Cached valid moves, this is calculated whenever the piece is grabbed
	*/
	std::vector<Move> validMoves;

	/*
		This is the thread used for calculating allowed moves
	*/
	std::thread moveProccessingThread;

	/*
		Boolean value indicating if the piece is virtually on the board
		during check detection, a piece may be virtually deleted to
		perform a check, it will always be restored before the function ends
	*/
	GLboolean virtualDeleted;
};


#endif