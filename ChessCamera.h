#ifndef CHESS_CAMERA_H
#define CHESS_CAMERA_H

#include"GlCamera.h"
#include"common.h"

//the distance along the xz plane from the (0,0) to the camera
#define RADIUS 3.0
//how high above the board the camera should rest
#define HEIGHT 4.5f

//the seconds the rotation animation should take
#define ROTATION_TIME 2.0;

/*
	This camera performs rotation animations between
	the white side of the board and the black side
*/
class ChessCamera : public GlCamera {
public:
	/*
		Initializes the camera
	*/
	ChessCamera(const GLuint width, const GLuint height);

	/*
		Does the camera movement to change side on the board
	*/
	virtual void frame();

	/*
		This class take 1 argument at data[0]
		of either WHITE or BLACK, definining the side of the
		board that should be looked at
		@param data
			The vector containing the color to look at
	*/
	virtual void passData(const std::vector<GLint> data);

	/*
		Kicks off an animation to look at the provided color
		@param color
			Either WHITE or BLACK
	*/
	void lookAt(GLboolean color);
private:

	/*
		The side of the board that should be looked at,
		either WHITE or BLACK
	*/
	GLboolean colorToLookAt;

	/*
		The angle between the -z axis and the  line on the xz place
		from (0, 0) to the camera

		this value is 0.0f when looking at white and
		PI when looking at black
	*/
	GLdouble angle;

	/*
		Indicates if the camera is moving or not
	*/
	GLboolean animationRunning;

	/*
		When the last frame ran for this camera
	*/
	GLdouble lastFrameTime;
};
#endif