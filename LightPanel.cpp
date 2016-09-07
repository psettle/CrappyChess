#include"LightPanel.h"


LightPanel::LightPanel(const ObjBinFile& file, const GLfloat boardWidth)
: GlObject(), visible(GL_FALSE) {
	this->addVertices(file.getVertexes());
	this->addNormals(file.getNormals());
	this->addUVs(file.getUVs());

	//scale down to 1 square, they are 2.0f wide to start with
	this->scale(boardWidth / 8.0f / 2.0f);
}

void LightPanel::isVisible(GLboolean visible) {
	this->visible = visible;
}

void LightPanel::draw() const {
	if (this->visible) {
		//this texture for this object is transparent, so we enable 
		//a blending function for it, then disable it afterwards
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		GlObject::draw();
		glDisable(GL_BLEND);
	}
}

GLfloat LightPanel::getBottomHeight() const {
	return 0.055180f * this->current_scale;
}