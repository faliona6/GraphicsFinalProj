#pragma once

#ifndef RIGIDBODY_H_INCLUDED
#define RIGIDBODY_H_INCLUDED

#include "Shape.h"
#include "Transform.h"
#include "Time.h";

using namespace std;
using namespace glm;

class RigidBody
{
public:
	RigidBody();
	RigidBody(shared_ptr<Time> time);
	~RigidBody();
	float speed;
	vec3 direction;
	float acceleration;
	float minX, maxX, minY, maxY;

	vec3 calcNextPos(vec3 curPos);

private:
	shared_ptr<Time> time;
};

#endif

