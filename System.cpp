#include"System.h"

GlSystem::GlSystem() {
	this->pWindowM = NULL;
}

GLboolean GlSystem::initialize(const size_t& width, const size_t& height, const std::string& name, const bool fullscreen) {
	//if glfw fails to initialize return failure
	if (!glfwInit()) {
		return false;
	}

	glfwWindowHint(GLFW_SAMPLES, 16); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL , this disables deprecated features to keep us honest

																   //create the window
	if (fullscreen) {
		this->pWindowM = glfwCreateWindow(width, height, name.c_str(), glfwGetPrimaryMonitor(), NULL);
	} else {
		this->pWindowM = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
	}

	//if the window creation failed, return failure
	if (this->pWindowM == NULL) {
		return false;
	}

	glfwMakeContextCurrent(this->pWindowM);

	glewExperimental = true;

	if (glewInit() != GLEW_OK) {
		return false;
	}

	//this keeps faces nearer to the camera on top
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//this tells openGL not to bother rendering faces that point away from us, it assumes
	//vertexes are labeled in the correct order (so cross product gets the right direction)
	//or that correct normals are provided for objects
	glEnable(GL_CULL_FACE);
	
	//disable vsync for unlimited frame rate
	glfwSwapInterval(1);

	//link the system for static callbacks
	handle = this;
	//tell the window not to close yet
	this->shouldClose = false;

	//this deletes the mouse cursor if its not required
	//glfwSetInputMode(this->pWindowM, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	//return success
	return true;
}

void GlSystem::addObject(GlObject* object) {
	object->setCamera(this->pCameraM);
	object->initialize();
	this->objectsM.push_back(object);
}

void GlSystem::run() {
	while (true) {
		this->frame();

		if (this->shouldClose) {
			break;
		}
	}
}

const bool GlSystem::frame() {
	glfwPollEvents();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	if (glfwGetKey(this->pWindowM, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(this->pWindowM) != 0) {
		this->shouldClose = true;
		return false;
	}

	//draw every object
	for (GLuint pos = 0; pos < this->objectsM.size(); pos++) {
		this->objectsM[pos]->draw();
	}
	//run the camera frame
	this->pCameraM->frame();

	//display the buffer
	glfwSwapBuffers(this->pWindowM);

	return true;
}

void GlSystem::shutdown() {
	handle = NULL;
	glfwTerminate();
}

GlSystem::~GlSystem() {	
	for (size_t obj = 0; obj < this->objectsM.size(); obj++) {
		delete this->objectsM[obj];
	}
	delete this->pCameraM;
}

Ray GlSystem::getWorldSpaceMouseRay() const {
	//get the cursor position
	GLdouble xpos, ypos;
	glfwGetCursorPos(this->pWindowM, &xpos, &ypos);
	//get the window size
	GLint width, height;
	glfwGetWindowSize(this->pWindowM, &width, &height);
	//normalize the cursor pos
	xpos = 2 * xpos / width - 1;
	ypos = -2 * ypos / height + 1;

	//create a ray in viewspace
	Ray ray;
	glm::vec4 start = glm::vec4(xpos, ypos, -1.0f, 1.0f);
	glm::vec4 end = glm::vec4(xpos, ypos, 0.0f, 1.0f);

	//get the camera to revert coordinates to worldspace
	this->pCameraM->rayToWorldspace(start, end);

	//now generate the normalized direction vector
	ray.direction = glm::normalize(glm::vec3(end - start));
	//and assign the start point
	ray.origin = glm::vec3(start);
	//return the ray
	return ray;
}

GlSystem* GlSystem::handle = NULL;