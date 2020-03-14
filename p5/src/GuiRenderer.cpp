#include "GuiRenderer.h"
#include "Program.h"

#include "GLSL.h"

using namespace glm;

GuiRenderer::GuiRenderer()
{
	vector<float> positions = { -1, 1, -1, -1, 1, 1, 1, -1 };
	quad = make_shared<Shape>();
	quad->createShape(positions);
	//quad->init();

	// Initialize the vertex array object
	glGenVertexArrays(1, &quad->vaoID);
	glBindVertexArray(quad->vaoID);

	// Send the position array to the GPU
	glGenBuffers(1, &quad->posBufID);
	glBindBuffer(GL_ARRAY_BUFFER, quad->posBufID);
	glBufferData(GL_ARRAY_BUFFER, quad->posBuf.size() * sizeof(float), &quad->posBuf[0], GL_STATIC_DRAW);

	// Unbind the arrays
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//CHECKED_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}


GuiRenderer::~GuiRenderer()
{
}

void GuiRenderer::render(vector<GuiTexture> guis, const shared_ptr<Program> prog) {

	glBindBuffer(GL_ARRAY_BUFFER, quad->posBufID);
	glBindVertexArray(quad->getVaoId());
	glEnableVertexAttribArray(0);
	// render
	for (GuiTexture gui : guis) {
		// 4 is for vertex count
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}
