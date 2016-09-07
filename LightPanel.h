#ifndef LIGHT_PANEL_H
#define LIGHT_PANEL_H

#include"GlObject.h"
#include"ObjBinFile.h"

class LightPanel : public GlObject {
public:
	/*
		Initializes the panel
		@param file
			The file to use as a source for data
	*/
	LightPanel(const ObjBinFile& file, const GLfloat boardWidth);

	/*
		Sets this panel to be visible/ invisible
		@param visible
			Indicates if the panel should be visible
	*/
	void isVisible(GLboolean visible);

	/*
		@see GlObject::draw()
		Overriding to make object invisible sometimes, and
		to make the texture transparent
	*/
	virtual void draw() const;

	/*
		Returns the height from the top of the object to its center
	*/
	GLfloat getBottomHeight() const;
protected:
	/*
		Indicates if the variable is currently rendered
	*/
	GLboolean visible;
};

#endif