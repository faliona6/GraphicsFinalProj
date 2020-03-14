#pragma once

#ifndef GAMEOBJECT_H_INCLUDED
#define GAMEOBJECT_H_INCLUDED

#include "Shape.h"
#include "Transform.h"
#include "Mesh.h"
#include "RigidBody.h"

class GameObject
{


public:
	GameObject(string meshName, string resourceDirectory);
	GameObject(string meshName, string textureName, string resourceDirectory);
	~GameObject();

	Transform transform;
	RigidBody rgbd;
	Mesh mesh;


	shared_ptr<Program> prog;

	void setTransform(Transform * t);
	void setMesh(Mesh * m);
	void Draw();
	void setTime(shared_ptr<Time> time);
	void createRigidBody();
	void updatePos();

private:
	shared_ptr<Time> time;
};

#endif

