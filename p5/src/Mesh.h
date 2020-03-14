#pragma once

#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include "Shape.h"
#include "Texture.h"
#include "Transform.h"
#include "MatrixStack.h"

class Mesh
{
public:
	Mesh();
	Mesh(string name, string resourceDirectory);
	Mesh(string meshName, string textureName, string resourceDirectory);
	~Mesh();
	void setTexture(string textureName);

	void InitTexture();
	void InitShape();

	void Draw(shared_ptr<Program> program, Transform transform);
	void Draw(shared_ptr<Program> program);

	vector<shared_ptr<Shape>> meshes;
	string meshName;
	shared_ptr<Texture> texture;

	int materialNum;

private:
	string resourceDirectory;
	string textureName;

	void setModel(std::shared_ptr<Program> prog, std::shared_ptr<MatrixStack>M);
	void SetMaterial(int i, shared_ptr<Program> prog);

};

#endif

