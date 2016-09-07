#include"ChessSystem.h"

ChessSystem::ChessSystem()
	: GlSystem(), pieces(), pGrabbed(NULL), pBoard(NULL), lastMouseState(false), whoseTurn(WHITE) {}

GLboolean ChessSystem::initialize(const size_t& width, const size_t& height, const bool fullscreen) {
	GLboolean success = GlSystem::initialize(width, height, "Chess", fullscreen);

	//create a camera for chess
	this->pCameraM = new ChessCamera(width, height);
	
	//initialize the light pointer
	this->lights = new std::vector<glm::vec3>();

	//add all the objects to the world
	this->addObjects();

	return success;
}

void ChessSystem::addObjects() {
	//this builds the shaders and textures we need for creating objects
	this->buildAssets();
	
	//create and add the board
	Board* board = new Board();
	board->setShader(this->shaders[LIGHT_SHADER]);
	board->setTexture(this->textures[BOARD_TEXTURE]);

	this->addBoard(board);

	//add all the pieces
	this->addPawns();
	this->addRooks();
	this->addBishops();
	this->addQueens();
	this->addKnights();
	this->addKings();
	this->addPanels();
}

void ChessSystem::buildAssets() {
	Shader* shader = new Shader("shaders/textureVertexShader.glsl", "shaders/textureFragmentShader.glsl");

	this->shaders.emplace(TEXTURE_SHADER, shader);

	shader = new Shader("shaders/directionalLightVertexShader.glsl", "shaders/directionalLightFragmentShader.glsl");

	this->shaders.emplace(LIGHT_SHADER, shader);

	Texture* texture = new Texture("textures/LightPanel.png", GL_TEXTURE0, this->shaders["textureShader"], "image1");
	this->textures.emplace(LIGHT_PANEL, texture);

	texture = new Texture("textures/blackPiece.png", GL_TEXTURE0, this->shaders["lightShader"], "image1");
	this->textures.emplace(BLACK_TEXTURE, texture);

	texture = new Texture("textures/whitePiece.png", GL_TEXTURE0, this->shaders["lightShader"], "image1");
	this->textures.emplace(WHITE_TEXTURE, texture);

	texture = new Texture("textures/boardv2Texture.png", GL_TEXTURE0, this->shaders["lightShader"], "image1");
	this->textures.emplace(BOARD_TEXTURE, texture);

	//now that all the objects are built, we assign them to ChessPiece
	//so it can create objects really quickly
	ChessPiece::white = this->textures[WHITE_TEXTURE];
	ChessPiece::black = this->textures[BLACK_TEXTURE];
	ChessPiece::shader = this->shaders[LIGHT_SHADER];
}

void ChessSystem::addPawns() {
	ObjBinFile pawnFile = ObjBinFile("models/pawn.objbin");

	GLfloat boardWidth = this->pBoard->getWidth();
	GLfloat boardHeight = this->pBoard->getTopHeight();

	Pawn* piece = NULL;

	for (GLint x = 0; x < 8; x++) {
		piece = new Pawn(pawnFile, WHITE, boardWidth, boardHeight);
		piece->setChessPosition(Move(x, 1));
		this->addPiece(piece);

		piece = new Pawn(pawnFile, BLACK, boardWidth, boardHeight);
		piece->setChessPosition(Move(x, 6));
		this->addPiece(piece);
	}
}

void ChessSystem::addRooks() {
	ObjBinFile rookFile = ObjBinFile("models/rook.objbin");

	GLfloat boardWidth = this->pBoard->getWidth();
	GLfloat boardHeight = this->pBoard->getTopHeight();

	Rook* piece = NULL;

	for (GLint x = 0; x < 8; x += 7) {
		piece = new Rook(rookFile, WHITE, boardWidth, boardHeight);
		piece->setChessPosition(Move(x, 0));
		this->addPiece(piece);

		piece = new Rook(rookFile, BLACK, boardWidth, boardHeight);
		piece->setChessPosition(Move(x, 7));
		this->addPiece(piece);
	}
}

void ChessSystem::addKnights() {
	ObjBinFile knightFile = ObjBinFile("models/knight.objbin");

	GLfloat boardWidth = this->pBoard->getWidth();
	GLfloat boardHeight = this->pBoard->getTopHeight();

	Knight* knight = NULL;

	knight = new Knight(knightFile, WHITE, boardWidth, boardHeight);
	knight->setChessPosition(Move(1, 0));
	this->addPiece(knight);

	knight = new Knight(knightFile, WHITE, boardWidth, boardHeight);
	knight->setChessPosition(Move(6, 0));
	this->addPiece(knight);

	knight = new Knight(knightFile, BLACK, boardWidth, boardHeight);
	knight->setChessPosition(Move(1, 7));
	this->addPiece(knight);

	knight = new Knight(knightFile, BLACK, boardWidth, boardHeight);
	knight->setChessPosition(Move(6, 7));
	this->addPiece(knight);
}

void ChessSystem::addBishops() {
	ObjBinFile bishopFile = ObjBinFile("models/bishop.objbin");

	GLfloat boardWidth = this->pBoard->getWidth();
	GLfloat boardHeight = this->pBoard->getTopHeight();

	Bishop* piece = NULL;

	piece = new Bishop(bishopFile, WHITE, boardWidth, boardHeight);
	piece->setChessPosition(Move(2, 0));
	this->addPiece(piece);

	piece = new Bishop(bishopFile, WHITE, boardWidth, boardHeight);
	piece->setChessPosition(Move(5, 0));
	this->addPiece(piece);

	piece = new Bishop(bishopFile, BLACK, boardWidth, boardHeight);
	piece->setChessPosition(Move(2, 7));
	this->addPiece(piece);

	piece = new Bishop(bishopFile, BLACK, boardWidth, boardHeight);
	piece->setChessPosition(Move(5, 7));
	this->addPiece(piece);
}

void ChessSystem::addQueens() {
	ObjBinFile queenFile = ObjBinFile("models/queen.objbin");

	GLfloat boardWidth = this->pBoard->getWidth();
	GLfloat boardHeight = this->pBoard->getTopHeight();

	Queen* queen = NULL;

	queen = new Queen(queenFile, WHITE, boardWidth, boardHeight);
	queen->setChessPosition(Move(4, 0));
	this->addPiece(queen);

	queen = new Queen(queenFile, BLACK, boardWidth, boardHeight);
	queen->setChessPosition(Move(3, 7));
	this->addPiece(queen);
}

void ChessSystem::addKings() {
	ObjBinFile kingFile = ObjBinFile("models/king.objbin");

	GLfloat boardWidth = this->pBoard->getWidth();
	GLfloat boardHeight = this->pBoard->getTopHeight();

	King* king = NULL;

	king = new King(kingFile, WHITE, boardWidth, boardHeight);
	king->setChessPosition(Move(3, 0));
	this->addPiece(king);

	king = new King(kingFile, BLACK, boardWidth, boardHeight);
	king->setChessPosition(Move(4, 7));
	this->addPiece(king);
}

void ChessSystem::addPanels() {
	ObjBinFile panelFile = ObjBinFile("models/lightpanel.objbin");

	GLfloat boardWidth = this->pBoard->getWidth();
	GLfloat boardHeight = this->pBoard->getTopHeight();

	GLfloat rowWidth = boardWidth / 8;
	GLfloat offset = rowWidth / 2;
	
	//generate all 64 light panels
	for (size_t i = 0; i < TILE_COUNT; i++) {
		GLint x = i / 8;
		GLint z = i % 8;

		GLfloat xPos = (x - 4) * rowWidth + offset;
		GLfloat zPos = (z - 4) * rowWidth + offset;

		LightPanel* panel = new LightPanel(panelFile, boardWidth);
		panel->setShader(this->shaders[TEXTURE_SHADER]);
		panel->setTexture(this->textures[LIGHT_PANEL]);
		
		panel->position(glm::vec3(xPos, this->pBoard->getTopHeight() - panel->getBottomHeight() + 0.0001f, zPos));
		this->lightPanels.push_back(panel);
		this->addObject(panel);
	}
}

void ChessSystem::addBoard(Board* board) {
	this->addObject(board); 
	this->pBoard = board;
	board->setLights(this->lights);
}

void ChessSystem::addPiece(ChessPiece* piece) {
	this->addObject(piece);
	this->pieces.push_back(piece);
	piece->setLights(this->lights);
}

const bool ChessSystem::frame() {
	this->processGameLogic();
	return GlSystem::frame();
}


void ChessSystem::processGameLogic() {
	//get left mouse button state
	GLuint stateLeft = glfwGetMouseButton(this->pWindowM, GLFW_MOUSE_BUTTON_LEFT);
	bool leftPressed = stateLeft == GLFW_PRESS;
	//if no object is bound, the left button is pressed, and it wasnt pressed last frame, attempt to grab a piece
	if (this->pGrabbed == NULL && leftPressed && !this->lastMouseState) {
		this->grabPiece();

	}
	//otherwise, if an object is bound and the button is still pressed, move it with the cursor
	else if (this->pGrabbed != NULL && leftPressed)
	{
		this->moveGrabbedPiece();
	}
	//lastly, if the object is grabbed, and the left button is not pressed, release the object
	else if (this->pGrabbed != NULL && !leftPressed)
	{
		this->releasePiece();
	} //else do nothing

	this->lastMouseState = leftPressed;
}

void ChessSystem::grabPiece() {
	Ray ray = this->getWorldSpaceMouseRay();

	GLfloat minDistance = 1000000.0f;

	//for each piece, test the intersect with the mouse, if it works
	//capture the distance and link the piece if its close than the last one
	for (GLuint piece = 0; piece < this->pieces.size(); piece++) {
		GLfloat distance;
		if (this->pieces[piece]->getColor() == this->whoseTurn && this->pieces[piece]->cursorIntersect(ray, distance)) {
			//if the hit piece is closer than the last one,
			//assign it as grabbed (for reference) and lower the minimum distance
			if (distance < minDistance) {
				this->pGrabbed = this->pieces[piece];
				minDistance = distance;
			}
		}
	}

	if (this->pGrabbed != NULL) {
		//set the grabbed location to where the ray hit the object,
		//that way when the mouse moves, we just set the intersect location on the modal
		//to the place where the new ray hits the board

		this->pGrabbed->getGrabbed(this->pieces); //launches a thread to calculate where the piece can go
												  //the move validation that occurs as the piece is placed will not run until that thread exits
		glm::vec3 intersect = ray.origin + minDistance * ray.direction;

		this->grabLocation = this->pGrabbed->worldToModel(intersect);
	}
}

void ChessSystem::moveGrabbedPiece() {
	ChessPiece* grabbed = this->pGrabbed;

	//grab the moves to highlight, before the move processing is done, this will just show nothing
	std::vector<Move> moves = grabbed->getValidMovesFromCache();
	//highlight all of the moves
	this->highlightMoves(moves);


	//first we need the new mouse board intersection 
	Ray ray = this->getWorldSpaceMouseRay();

	//calculate the  distance to the board
	GLfloat boardIntersect;
	this->pBoard->cursorIntersect(ray, boardIntersect);

	//calculate the intersection with the board
	glm::vec3 intersect = ray.origin + boardIntersect * ray.direction;
	//now calculate the distance required to be this->grabLocation.y above the board
	//we shift the position back to worldspace, and substract the boards height, leaving only the difference
	//between where it was clicked and the top of the board
	GLfloat height = grabbed->modelToWorld(this->grabLocation).y - this->pBoard->getTopHeight();
	//get the distance that need to be traveled back to get to the calculated height
	GLfloat distanceAlongPath = fabs(height / ray.direction.y);
	//get a new coordiate by backtracking along the ray
	glm::vec3 intersectAtHeight = intersect - distanceAlongPath * ray.direction;


	//now move the box to the intersect at height position, then shift

	//this places the center of the object at the same height it was grabbed at
	grabbed->position(intersectAtHeight);
	//we translate by the difference between its grabbed position in worldspace and its actual position in world space
	//this gets it back onto the board instead of above
	grabbed->translate(grabbed->getPosition() - grabbed->modelToWorld(this->grabLocation));
}

void ChessSystem::releasePiece() {
	//clear the highlighted moves
	this->clearHighlightedMoves();

	//first we need to figure out which grid position the new position is over,
	Move position;
	this->pGrabbed->getChessPosition(position);

	std::vector<ChessPiece*> otherPieces = this->pieces;

	//this function blocks the main threads execution until the thread that was started to
	//calculate the legal move finishes
	this->pGrabbed->onBeforeValidMove();
	//now check if move is legal, if its not, revert the position and release the piece
	if (!this->pGrabbed->validMove(position)) {
		this->pGrabbed->revertPosition();
		this->pGrabbed = NULL;
		return;
	}

	//set the piece into position
	std::vector<ChessPiece*> toDelete = this->pGrabbed->makeMove(otherPieces, position);

	//delete the pieces that the moved piece requested
	for (size_t piece = 0; piece < toDelete.size(); piece++) {
		this->removePieceFromSystem(toDelete[piece]);
	}

	//release the piece
	this->pGrabbed = NULL;

	//switch the turn
	this->whoseTurn = !this->whoseTurn;

	//move the camera
	std::vector<GLint> data;
	data.push_back(this->whoseTurn);
	this->pCameraM->passData(data);
}

void ChessSystem::removePieceFromSystem(ChessPiece* piece) {
	//there are 2 places to remove the piece from, the object array,and the piece array
	for (size_t i = 0; i < this->objectsM.size(); i++) {
		if (this->objectsM[i] == piece) {
			this->objectsM.erase(this->objectsM.begin() + i);
			i--;
		}
	}

	for (size_t i = 0; i < this->pieces.size(); i++) {
		if (this->pieces[i] == piece) {
			this->pieces.erase(this->pieces.begin() + i);
			i--;
		}
	}

	delete piece;
}

void ChessSystem::highlightMoves(const std::vector<Move> moves) {
	//we will only apply the moves if they exist, i.e moves != 0 and
	//there arent lights already set, to reset move this->clearHighlightedMoves must be called
	if (moves.size() == 0) {
		return;
	} else if (this->lights->size() != 0) {
		return;
	}
	
	GLfloat boardWidth = this->pBoard->getWidth();
	GLfloat boardHeight = this->pBoard->getTopHeight();

	GLfloat rowWidth = boardWidth / 8;
	GLfloat offset = rowWidth / 2;

	for (size_t i = 0; i < moves.size(); i++) {
		GLfloat xPos = (moves[i].x - 4) * rowWidth + offset;
		GLfloat zPos = (moves[i].z - 4) * rowWidth + offset;

		this->lights->push_back(glm::vec3(xPos, boardHeight + 0.05f, zPos));
		this->lightPanels[moves[i].x * 8 + moves[i].z]->isVisible(GL_TRUE);
	}
}

void ChessSystem::clearHighlightedMoves() {
	//empty the light vector and mark each panel as  empty
	this->lights->clear();
	for (size_t i = 0; i < TILE_COUNT; i++) {
		this->lightPanels[i]->isVisible(GL_FALSE);
	}
}

ChessSystem::~ChessSystem() {
	delete this->lights;

	//iterate over the maps to delete the shaders and textures
	for (std::map<std::string, Shader*>::iterator iterator = this->shaders.begin(); iterator != this->shaders.end(); iterator++) {
		delete iterator->second;
	}

	for (std::map<std::string, Texture*>::iterator iterator = this->textures.begin(); iterator != this->textures.end(); iterator++) {
		delete iterator->second;
	}
}

void ChessSystem::install() {
	/*
		This function simply loads every model from a .obj file
		and saves it as a .objbin file (the .objbin file can be loaded much more quickly)
	*/
	
	ObjBinFile file;

	std::cout << "Compiling Board... starting..." << std::endl;
	file.loadObjFile("models/board.obj");
	file.saveObjBinFile("models/board.objbin");
	std::cout << "Compiling Board... done..." << std::endl;

	std::cout << "Compiling Pawn... starting..." << std::endl;
	file.loadObjFile("models/pawn.obj");
	file.saveObjBinFile("models/pawn.objbin");
	std::cout << "Compiling Pawn... done..." << std::endl;

	std::cout << "Compiling Knight... starting..." << std::endl;
	file.loadObjFile("models/knight.obj");
	file.saveObjBinFile("models/knight.objbin");
	std::cout << "Compiling Knight... done..." << std::endl;

	std::cout << "Compiling Rook... starting..." << std::endl;
	file.loadObjFile("models/rook.obj");
	file.saveObjBinFile("models/rook.objbin");
	std::cout << "Compiling Rook... done..." << std::endl;

	std::cout << "Compiling Bishop... starting..." << std::endl;
	file.loadObjFile("models/bishop.obj");
	file.saveObjBinFile("models/bishop.objbin");
	std::cout << "Compiling Bishop... done..." << std::endl;

	std::cout << "Compiling Queen... starting..." << std::endl;
	file.loadObjFile("models/queen.obj");
	file.saveObjBinFile("models/queen.objbin");
	std::cout << "Compiling Queen... done..." << std::endl;

	std::cout << "Compiling King... starting..." << std::endl;
	file.loadObjFile("models/king.obj");
	file.saveObjBinFile("models/king.objbin");
	std::cout << "Compiling King... done..." << std::endl;

	std::cout << "Compiling Panels... starting..." << std::endl;
	file.loadObjFile("models/LightPanel.obj");
	file.saveObjBinFile("models/lightpanel.objbin");
	std::cout << "Compiling Panels... done..." << std::endl;
}
