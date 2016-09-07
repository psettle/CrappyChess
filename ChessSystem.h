#ifndef CHESS_SYSTEM_H
#define CHESS_SYSTEM_H
#include"System.h"
#include"Board.h"
#include"Pawn.h"
#include"Queen.h"
#include"Rook.h"
#include"Bishop.h"
#include"Knight.h"
#include"King.h"
#include"ChessCamera.h"
#include"LightPanel.h"

//keys for loaded shaders
#define TEXTURE_SHADER "textureShader"
#define LIGHT_SHADER "lightShader"
//keys for loaded textures
#define LIGHT_PANEL "lightPanel"
#define BLACK_TEXTURE "black"
#define WHITE_TEXTURE "white"
#define BOARD_TEXTURE "board"

//how many tiles are on a chess board
#define TILE_COUNT 64

class ChessSystem : public GlSystem {
public:
	
	/*
		Safely initialize member variables
	*/
	ChessSystem();

	/*
		@see GlSystem::initialize()
	*/
	virtual GLboolean initialize(const size_t& width, const size_t& height, const bool fullscreen);

	/*
		Compiles the models into the engine's binary format
	*/
	static void install();

	/*
		Deletes system resources
	*/
	virtual ~ChessSystem();

private:
	/*
		Adds a piece to the boards internal memory, this class will delete the object
		when it is destroyed
		@param piece
			piece to be added
	*/
	void addPiece(ChessPiece* piece);

	/*
		Adds a board to the system internal memory, this class will delete the object
		when it is destroyed
		@param board
			board to be used
	*/
	void addBoard(Board* board);

	/*
		Builds the shaders and textures for the game
	*/
	void buildAssets();

	/*
		Adds all the openGL object onto into the game
	*/
	void addObjects();

	/*
		Adds all the pawns to the board
	*/
	void addPawns();

	/*
		Adds all the rooks to the board
	*/
	void addRooks();

	/*
		Adds all the knights to the board
	*/
	void addKnights();

	/*
		Adds all the bishops to the board
	*/
	void addBishops();

	/*
		Adds all the queens to the board
	*/
	void addQueens();

	/*
		Adds all the kings to the board
	*/
	void addKings();

	/*
		Adds all the direction panels to the board
	*/
	void addPanels();

	/*
		Runs a single frame of the application
	*/
	virtual const bool frame();

	/*
		Processes user input and game logic 
	*/
	void processGameLogic();

	/*
		Moves the currently bound piece to the mouse location
	*/
	void moveGrabbedPiece();

	/*
		Attempts to grab a piece from the mouse and binds it to this object
	*/
	void grabPiece();

	/*
		Clears the bound piece and places back onto the board, 
		if the user selected move is illegal, returns the piece to its initial position
			checks for capture,
			checks for entering check
	*/
	void releasePiece();

	
	/*
		Remove the provided piece completely from the system
		@param piece
			The piece to remove
	*/
	void removePieceFromSystem(ChessPiece* piece);

	/*
		Highlights all of the provided moves on the board
		@param moves
			the vector of moves to make
	*/
	void highlightMoves(const std::vector<Move> moves);

	/*
		Clears all of the highlighted moves
	*/
	void clearHighlightedMoves();

	/*
		The object (piece) currently bound to the cursor, null if none is set
	*/
	ChessPiece* pGrabbed;

	/*
		The position in model space that the currently bound piece was grabbed on, 
		this is used to keep the piece allignment when dragging
	*/
	glm::vec3 grabLocation;

	/*
		Reference to the board
	*/
	Board* pBoard;

	/*
		The pieces on the board
	*/
	std::vector<ChessPiece*> pieces;

	/*
		The left mouse button state during the last frame, this is used to detect when a click occurrs
	*/
	GLboolean lastMouseState;

	/*
		Which colors turn it is
	*/
	GLboolean whoseTurn;

	/*
		A vector of places to put point lights
	*/
	std::vector<glm::vec3>* lights;

	/*
		A vector of the panels, the panel under 8x + z is at (x, z)
	*/
	std::vector<LightPanel*> lightPanels;

	/*
		Shaders to use then delete
	*/
	std::map<std::string, Shader*> shaders;

	/*
		Textures to use then delete
	*/
	std::map<std::string, Texture*> textures;
};

#endif