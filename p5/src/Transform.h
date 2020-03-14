#pragma once

#ifndef TRANSFORM_H_INCLUDED
#define TRANSFORM_H_INCLUDED

#include "Shape.h"

class Transform;

using namespace std;
using namespace glm;


class Transform
{
public:
	~Transform();
	Transform();
	Transform(vec3 position);
	vec3 position;
	vec3 scale;
	vec3 rotation;

	void updatePos();
	vec3 getForward();


};

#endif

