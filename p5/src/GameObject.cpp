#include "GameObject.h"
#include "Shape.h"
#include "Transform.h"
#include "Mesh.h"
#include "Time.h"

GameObject::GameObject(string meshName, string resourceDirectory)
{
	transform = Transform();
	mesh = Mesh(meshName, resourceDirectory);
}

GameObject::GameObject(string meshName, string textureName, string resourceDirectory)
{
	transform = Transform();
	mesh = Mesh(meshName, textureName, resourceDirectory);
}


GameObject::~GameObject()
{
}

void GameObject::setTransform(Transform * t)
{
	transform = *t;
}

void GameObject::setMesh(Mesh * m)
{
	mesh = *m;
}

void GameObject::setTime(shared_ptr<Time> time)
{
	this->time = time;
}

//TODO : check if time is null
void GameObject::createRigidBody()
{
	rgbd = RigidBody(this->time);
}

void GameObject::Draw()
{
	mesh.Draw(prog, transform);
}


void GameObject::updatePos()
{
	transform.position = rgbd.calcNextPos(transform.position);
}

