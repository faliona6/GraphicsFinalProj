#pragma once
#include "Shape.h"

class Program;

class Terrain
{
public:
	Terrain();
	~Terrain();
	shared_ptr<Shape> generateTerrain();

private:
	static const int SIZE = 300;
	static const int VERTEX_COUNT = 64;

	float x, z;

};

