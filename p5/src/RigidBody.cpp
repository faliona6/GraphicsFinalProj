#include "RigidBody.h"
#include "Transform.h"
#include "Time.h";

RigidBody::RigidBody()
{
	direction = vec3(0, 0, 0);
	acceleration = minX = maxX = minY = maxY = speed = 0;
}

RigidBody::RigidBody(shared_ptr<Time> time)
{
	direction = vec3(0, 0, 0);
	acceleration = minX = maxX = minY = maxY = speed = 0;
	this->time = time;
}


RigidBody::~RigidBody()
{
}

vec3 RigidBody::calcNextPos(vec3 curPos)
{
	vec3 newPos = curPos + (direction * speed * time->deltaTime);
	
	return newPos;
}

