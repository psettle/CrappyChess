#ifndef GL_SYSTEM_H
#define GL_SYSTEM_H

#include"GlObject.h"

/*
Main system driver for openGL applications
*/
class GlSystem {
public:
	/*
		Safely initializes members
	*/
	GlSystem();

	/*
		Builds and configures the application window,
		no camera is set, so applications using this class should extend the class and  add a camera somewhere
		@param width
			width in pixels of the created window
		@param height
			height in pixels of the created window
		@param name
			the name associated with the created window
		@param fullscreen
			Indicates if a window should be created or if the window should be made full screen
		@return 
			true for successful initialization, false for failure
	*/
	virtual GLboolean initialize(const size_t& width, const size_t& height, const std::string& name, const bool fullscreen = true);

	/*
		Adds an object to the system,links the camera and initializes the object
		@param object
			An object of class GlObject (or derived type) that hasn't been initialized

	*/
	void addObject(GlObject* object);

	/*
		Runs the application
	*/
	void run();

	/*
		Safely disposes of system resources
	*/
	void shutdown();

	/*
		Safely deletes members
	*/
	~GlSystem();

	/*
	`Static handle to the system, used to handle static calls in an OO way
	*/
	static GlSystem* handle;
protected:
	/*
		Creates and returns a ray object from the camera through the mouse
		REQUIRES
			this->pCameraM must be set
		@return
			A ray object describing a ray from the camera through the mouse
	*/
	Ray getWorldSpaceMouseRay() const;
	
	/*
		Runs a single frame of the applicaton
	*/
	virtual const bool frame();

	/*
		Handle to the window
	*/
	GLFWwindow* pWindowM;

	/*
		Flag to check if this window should shutdown
	*/
	bool shouldClose;

	/*
		The camera to view the system through
	*/
	GlCamera* pCameraM;

	/*
		The objects the system is currently handling
	*/
	std::vector<GlObject*> objectsM;

	/*
		The Frame Buffer ID openGL allocates for the shadow mapping
	*/
	GLuint shadowDepthMapFBO;

	/*
		The texture ID openGL allocates for the depth map
	*/
	GLuint shadowDepthMap;

	/*
		Indicates if the object casts shadows
	*/
	GLboolean shadowEnabled;
};

#endif