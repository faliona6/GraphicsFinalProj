#pragma once



#ifndef GUI_REDNERER_H_INCLUDED
#define GUI_REDNERER_H_INCLUDED
#include "Shape.h"
#include "GuiTexture.h"


class GuiRenderer
{
public:
	GuiRenderer();
	~GuiRenderer();
	void render(vector<GuiTexture> guis, shared_ptr<Program> prog);

private:
	shared_ptr<Shape> quad;

};

#endif

