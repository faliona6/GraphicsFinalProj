#include <iostream>
#include <glad/glad.h>
#include <algorithm> 
#include <ctime>


#include "GuiTexture.h"
#include "Shape.h"
#include "GLSL.h"
#include "Program.h"
#include "Texture.h"

#include <vector>

using namespace std;
using namespace glm;

GuiTexture::GuiTexture(shared_ptr<Texture> texture, vec2 position, vec2 scale)
{
	this->texture = texture;
	this->position = position;
	this->scale = scale;
}


GuiTexture::~GuiTexture()
{
}

shared_ptr<Texture> GuiTexture::getTexture()
{
	return texture;
}
vec2 GuiTexture::getPos()
{
	return position;
}
vec2 GuiTexture::getScale()
{
	return scale;
}

