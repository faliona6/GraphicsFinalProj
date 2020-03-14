#pragma once

#ifndef GUITEXTURE_H_INCLUDED
#define GUITEXTURE_H_INCLUDED

#include "Shape.h"
#include "Texture.h"


class GuiTexture;

using namespace std;
using namespace glm;


class GuiTexture
{
public:
	GuiTexture(shared_ptr<Texture> texture, vec2 position, vec2 scale);
	~GuiTexture();

	shared_ptr<Texture> getTexture();
	vec2 getPos();
	vec2 getScale();


private:
	static const int SIZE = 300;
	static const int VERTEX_COUNT = 64;

	shared_ptr<Texture> texture;
	vec2 position;
	vec2 scale;

};

#endif

