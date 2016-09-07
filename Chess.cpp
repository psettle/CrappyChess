/*
ABOUT
This is a a chess game played locally, built using openGL
STARTED
Aug 27th, 2016

TODO
	Checkmate UI + new Game
	Pawn upgrade at finish line
	better models	

AUTHOR
Patrick Settle (patrick.settle@ucalgary.ca)

LIBRARIES USED
openGL
glfw
glew
SOIL
glm
*/

#include"ChessSystem.h"

/*
	Entry point for the game, initialize the system and then launches the game loop,
	safely deletes resources when the game ends/ is closed
*/
int main() {

#if 1//create the game manager
	ChessSystem system;
	//initialize the game manager
	GLboolean success = system.initialize(1920, 1080, true);

	const GLubyte* version = glGetString(GL_VERSION);
	std::cout << version << std::endl;

	//run the game
	if (success) {
		system.run();
	}
	//close the game
	system.shutdown();
#else
	ChessSystem::install();
#endif
	return 0;
}
