#include "Mesh.h"
#include "Program.h"
#include "Transform.h"
#include "MatrixStack.h"

#include <ctime>
#include <iostream>

using namespace std;

Mesh::Mesh()
{
	this->meshName = "";
	materialNum = 0;
}

Mesh::~Mesh()
{
}

Mesh::Mesh(string meshName, string resourceDirectory)
{
	this->meshName = meshName;
	this->resourceDirectory = resourceDirectory;
	InitShape();
}

Mesh::Mesh(string meshName, string textureName, string resourceDirectory)
{
	this->meshName = meshName;
	this->textureName = textureName;
	this->resourceDirectory = resourceDirectory;
	InitTexture();
	InitShape();
}

void Mesh::setTexture(string textureName)
{
	this->textureName = textureName;
	InitTexture();
}

void Mesh::InitTexture() {
	texture = make_shared<Texture>();
	texture->setFilename(resourceDirectory + "/" + textureName);
	texture->init();
	texture->setUnit(1);
	texture->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
}

void Mesh::InitShape()
{
	string errStr;
	vector<tinyobj::shape_t> TOshapes;
	vector<tinyobj::material_t> objMaterials;

	bool rc = tinyobj::LoadObj(TOshapes, objMaterials, errStr, (resourceDirectory + "/" + meshName).c_str());

	for (int j = 0; j < TOshapes.size(); j++) {
		shared_ptr<Shape> shape = make_shared<Shape>();
		if (!rc) {
			cerr << errStr << endl;
		}
		else {
			shape->createShape(TOshapes[j]);
			shape->measure();
			if (meshName.compare("simple_water.obj")) {
				shape->getWaterIndicators();
			}
			shape->init();
		}
		meshes.push_back(shape);
	}
}

void Mesh::Draw(shared_ptr<Program> prog, Transform transform)
{
	prog->bind();
	shared_ptr<MatrixStack> Model = make_shared<MatrixStack>();
	texture->bind(prog->getUniform("Texture0"));
	Model->pushMatrix();
		Model->translate(transform.position);
		Model->scale(transform.scale);
		Model->rotate(transform.rotation.x, vec3(1, 0, 0));
		Model->rotate(transform.rotation.y, vec3(0, 1, 0));
		Model->rotate(transform.rotation.z, vec3(0, 0, 1));
		setModel(prog, Model);
		SetMaterial(materialNum, prog);
		for (shared_ptr<Shape> mesh : meshes) {
			mesh->draw(prog);
		}
	Model->popMatrix();
	prog->unbind();
}

// Draw without any transforms
void Mesh::Draw(shared_ptr<Program> prog)
{
	prog->bind();
	if (texture != NULL)
		texture->bind(prog->getUniform("Texture0"));
	SetMaterial(materialNum, prog);
	for (shared_ptr<Shape> mesh : meshes) {
		mesh->draw(prog);
	}
	prog->unbind();
}

void Mesh::setModel(shared_ptr<Program> prog, shared_ptr<MatrixStack>M) {
	glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
}

void Mesh::SetMaterial(int i, shared_ptr<Program> prog) {
	switch (i) {
	case 0: //shiny blue plastic
		glUniform3f(prog->getUniform("matAmb"), 0.02, 0.04, 0.2);
		glUniform3f(prog->getUniform("difCol"), 0.0, 0.16, 0.9);
		glUniform3f(prog->getUniform("matSpec"), 0.14, 0.2, 0.8);
		glUniform1f(prog->getUniform("shine"), 60.0);
		break;
	case 1: // flat grey
		glUniform3f(prog->getUniform("matAmb"), 0.13, 0.13, 0.14);
		glUniform3f(prog->getUniform("difCol"), 0.1, 0.8, 0.1);
		glUniform3f(prog->getUniform("matSpec"), 0.3, 0.3, 0.4);
		glUniform1f(prog->getUniform("shine"), 4.0);
		break;
	case 2: //brass
		glUniform3f(prog->getUniform("matAmb"), 0.3294, 0.2235, 0.02745);
		glUniform3f(prog->getUniform("difCol"), 0.7804, 0.5686, 0.11373);
		glUniform3f(prog->getUniform("matSpec"), 0.9922, 0.941176, 0.80784);
		glUniform1f(prog->getUniform("shine"), 100.0);
		break;
	case 3: //grass
		glUniform3f(prog->getUniform("matAmb"), 0.1, 0.2, 0.1);
		glUniform3f(prog->getUniform("difCol"), 0.1, 0.8, 0.3);
		glUniform3f(prog->getUniform("matSpec"), 0.9922, 0.941176, 0.80784);
		glUniform1f(prog->getUniform("shine"), 100.0);
		break;
	case 4: //water
		glUniform3f(prog->getUniform("matAmb"), 0.3, 0.5, 0.8);
		glUniform3f(prog->getUniform("difCol"), 0.3, 0.6, 0.8);
		glUniform3f(prog->getUniform("matSpec"), 0.2, 0.3, 0.8);
		glUniform1f(prog->getUniform("shine"), 2.0);
		break;
	case 5: //bunny
		glUniform3f(prog->getUniform("matAmb"), 0.3, 0.1, 0.1);
		glUniform3f(prog->getUniform("difCol"), 0.8, 0.3, 0.3);
		glUniform3f(prog->getUniform("matSpec"), 0.5, 0.4, 0.4);
		glUniform1f(prog->getUniform("shine"), 4.0);
		break;
	}
}



