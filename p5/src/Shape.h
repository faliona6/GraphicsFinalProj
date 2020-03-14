
#pragma once

#ifndef LAB471_SHAPE_H_INCLUDED
#define LAB471_SHAPE_H_INCLUDED

#include <string>
#include <vector>
#include <memory>
#include <glm/gtc/type_ptr.hpp>
#include <tiny_obj_loader/tiny_obj_loader.h>

class Program;

using namespace std;
class Shape
{

public:

	void createShape(tinyobj::shape_t & shape);
	void createShape(vector<float> positions, vector<float> normals,
		vector<float> texcoords, vector<unsigned int> indices);
	void createShape(vector<float> positions);
	void init();
	void measure();
	void draw(const std::shared_ptr<Program> prog) const;
	void getNormals();
	void getWaterIndicators();
	unsigned int getVaoId();
	unsigned int vaoID = 0;
	unsigned int posBufID = 0;
	std::vector<float> posBuf;



	glm::vec3 min = glm::vec3(0);
	glm::vec3 max = glm::vec3(0);

private:

	std::vector<unsigned int> eleBuf;
	std::vector<float> norBuf;
	std::vector<float> texBuf;
	std::vector<float> indicatorBuf;
	unsigned int eleBufID = 0;

	unsigned int norBufID = 0;
	unsigned int texBufID = 0;
	unsigned int indicatorBufID = 0;

};

#endif // LAB471_SHAPE_H_INCLUDED
