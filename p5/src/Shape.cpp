
#include "Shape.h"
#include <iostream>
#include <cassert>

#include "GLSL.h"
#include "Program.h"
#define NOMINMAX

#include <algorithm> 

using namespace std;
using namespace glm;


// copy the data from the shape to this object
void Shape::createShape(tinyobj::shape_t & shape)
{
	posBuf = shape.mesh.positions;
	norBuf = shape.mesh.normals;
	texBuf = shape.mesh.texcoords;
	eleBuf = shape.mesh.indices;

	getNormals();
}

// copy the data from the shape to this object
void Shape::createShape(vector<float> positions)
{
	posBuf = positions;
}

void Shape::createShape(vector<float> positions, vector<float> normals,
	vector<float> texcoords, vector<unsigned int> indices)
{
	posBuf = positions;
	norBuf = normals;
	texBuf = texcoords;
	eleBuf = indices;

	getNormals();
}

void Shape::measure()
{
	float minX, minY, minZ;
	float maxX, maxY, maxZ;

	minX = minY = minZ = 1.1754E+38F;
	maxX = maxY = maxZ = -1.1754E+38F;

	//Go through all vertices to determine min and max of each dimension
	for (size_t v = 0; v < posBuf.size() / 3; v++)
	{
		if (posBuf[3*v+0] < minX) minX = posBuf[3 * v + 0];
		if (posBuf[3*v+0] > maxX) maxX = posBuf[3 * v + 0];

		if (posBuf[3*v+1] < minY) minY = posBuf[3 * v + 1];
		if (posBuf[3*v+1] > maxY) maxY = posBuf[3 * v + 1];

		if (posBuf[3*v+2] < minZ) minZ = posBuf[3 * v + 2];
		if (posBuf[3*v+2] > maxZ) maxZ = posBuf[3 * v + 2];
	}

	min.x = minX;
	min.y = minY;
	min.z = minZ;
	max.x = maxX;
	max.y = maxY;
	max.z = maxZ;
}

void Shape::getNormals()
{
	if (norBuf.empty()) {
		// Initialize the norBuf
		norBuf.assign(posBuf.size(), 0);

		// For every face
		for (size_t f = 0; f < eleBuf.size() / 3; f++) {
			int v1_i = eleBuf[3 * f + 0];
			int v2_i = eleBuf[3 * f + 1];
			int v3_i = eleBuf[3 * f + 2];

			vec3 v1 = vec3(posBuf[3 * v1_i + 0], posBuf[3 * v1_i + 1], posBuf[3 * v1_i + 2]);
			vec3 v2 = vec3(posBuf[3 * v2_i + 0], posBuf[3 * v2_i + 1], posBuf[3 * v2_i + 2]);
			vec3 v3 = vec3(posBuf[3 * v3_i + 0], posBuf[3 * v3_i + 1], posBuf[3 * v3_i + 2]);

			vec3 normal = cross(v2 - v1, v3 - v1);

			norBuf[3 * v1_i + 0] += normal.x;
			norBuf[3 * v1_i + 1] += normal.y;
			norBuf[3 * v1_i + 2] += normal.z;

			norBuf[3 * v2_i + 0] += normal.x;
			norBuf[3 * v2_i + 1] += normal.y;
			norBuf[3 * v2_i + 2] += normal.z;

			norBuf[3 * v3_i + 0] += normal.x;
			norBuf[3 * v3_i + 1] += normal.y;
			norBuf[3 * v3_i + 2] += normal.z;
		}
		// Calculate the normal
		// Add face normal to every vertex index of that face
	}
}

void Shape::getWaterIndicators()
{
	if (indicatorBuf.empty()) {
		// Initialize the norBuf
		indicatorBuf.assign(int(posBuf.size() * (4.0/3.0)), 0);

		// For every face
		for (size_t f = 0; f < eleBuf.size() / 3; f++) {
			int v1_i = eleBuf[3 * f + 0];
			int v2_i = eleBuf[3 * f + 1];
			int v3_i = eleBuf[3 * f + 2];

			vec3 v1 = vec3(posBuf[3 * v1_i + 0], posBuf[3 * v1_i + 1], posBuf[3 * v1_i + 2]);
			vec3 v2 = vec3(posBuf[3 * v2_i + 0], posBuf[3 * v2_i + 1], posBuf[3 * v2_i + 2]);
			vec3 v3 = vec3(posBuf[3 * v3_i + 0], posBuf[3 * v3_i + 1], posBuf[3 * v3_i + 2]);

			vec3 normal = cross(v2 - v1, v3 - v1);

			indicatorBuf[4 * v1_i + 0] = v2.x;
			indicatorBuf[4 * v1_i + 1] = v2.y;
			indicatorBuf[4 * v1_i + 2] = v3.x;
			indicatorBuf[4 * v1_i + 3] = v3.y;

			indicatorBuf[4 * v2_i + 0] = v1.x;
			indicatorBuf[4 * v2_i + 1] = v1.y;
			indicatorBuf[4 * v2_i + 2] = v3.x;
			indicatorBuf[4 * v2_i + 3] = v3.y;

			indicatorBuf[4 * v3_i + 0] = v2.x;
			indicatorBuf[4 * v3_i + 1] = v2.y;
			indicatorBuf[4 * v3_i + 2] = v1.x;
			indicatorBuf[4 * v3_i + 3] = v1.y;

		}
		// Calculate the normal
		// Add face normal to every vertex index of that face
	}
	/*
	CHECKED_GL_CALL(glGenBuffers(1, &indicatorBufID));
	CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, indicatorBufID));
	CHECKED_GL_CALL(glBufferData(GL_ARRAY_BUFFER, indicatorBuf.size() * sizeof(float), &indicatorBuf[0], GL_STATIC_DRAW));

	CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	CHECKED_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));*/
}


void Shape::init()
{
	// Initialize the vertex array object
	CHECKED_GL_CALL(glGenVertexArrays(1, &vaoID));
	CHECKED_GL_CALL(glBindVertexArray(vaoID));

	// Send the position array to the GPU
	CHECKED_GL_CALL(glGenBuffers(1, &posBufID));
	CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posBufID));
	CHECKED_GL_CALL(glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW));

	// Send the normal array to the GPU
	if (norBuf.empty())
	{
		norBufID = 0;
	}
	else
	{
		CHECKED_GL_CALL(glGenBuffers(1, &norBufID));
		CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, norBufID));
		CHECKED_GL_CALL(glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW));
	}

	// Send the texture array to the GPU
	if (texBuf.empty())
	{
		texBufID = 0;
	}
	else
	{
		CHECKED_GL_CALL(glGenBuffers(1, &texBufID));
		CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, texBufID));
		CHECKED_GL_CALL(glBufferData(GL_ARRAY_BUFFER, texBuf.size()*sizeof(float), &texBuf[0], GL_STATIC_DRAW));
	}

	// Send the water indicator array to the GPU
	if (indicatorBuf.empty())
	{
		indicatorBufID = 0;
	}
	else
	{
		CHECKED_GL_CALL(glGenBuffers(1, &indicatorBufID));
		CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, indicatorBufID));
		CHECKED_GL_CALL(glBufferData(GL_ARRAY_BUFFER, indicatorBuf.size() * sizeof(float), &indicatorBuf[0], GL_STATIC_DRAW));
	}

	// Send the element array to the GPU
	CHECKED_GL_CALL(glGenBuffers(1, &eleBufID));
	CHECKED_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eleBufID));
	CHECKED_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, eleBuf.size()*sizeof(unsigned int), &eleBuf[0], GL_STATIC_DRAW));

	// Unbind the arrays
	CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	CHECKED_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

unsigned int Shape::getVaoId()
{
	return vaoID;
}

void Shape::draw(const shared_ptr<Program> prog) const
{
	int h_pos, h_nor, h_tex;
	h_pos = h_nor = h_tex = -1;

	CHECKED_GL_CALL(glBindVertexArray(vaoID));

	// Bind position buffer
	h_pos = prog->getAttribute("vertPos");
	GLSL::enableVertexAttribArray(h_pos);
	CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posBufID));
	CHECKED_GL_CALL(glVertexAttribPointer(h_pos, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0));

	// Bind normal buffer
	h_nor = prog->getAttribute("vertNor");
	if (h_nor != -1 && norBufID != 0)
	{
		GLSL::enableVertexAttribArray(h_nor);
		CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, norBufID));
		CHECKED_GL_CALL(glVertexAttribPointer(h_nor, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0));
	}

	if (texBufID != 0)
	{
		// Bind texcoords buffer
		h_tex = prog->getAttribute("vertTex");

		if (h_tex != -1 && texBufID != 0)
		{
			GLSL::enableVertexAttribArray(h_tex);
			CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, texBufID));
			CHECKED_GL_CALL(glVertexAttribPointer(h_tex, 2, GL_FLOAT, GL_FALSE, 0, (const void *)0));
		}
	}

	if (indicatorBufID != 0)
	{
		// Bind texcoords buffer
		h_tex = prog->getAttribute("indicators");


		if (h_tex != -1 && indicatorBufID != 0)
		{
			GLSL::enableVertexAttribArray(h_tex);
			CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, indicatorBufID));
			CHECKED_GL_CALL(glVertexAttribPointer(h_tex, 4, GL_FLOAT, GL_FALSE, 0, (const void *)0));
		}
	}

	// Bind element buffer
	CHECKED_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eleBufID));

	// Draw
	CHECKED_GL_CALL(glDrawElements(GL_TRIANGLES, (int)eleBuf.size(), GL_UNSIGNED_INT, (const void *)0));

	// Disable and unbind
	if (h_tex != -1)
	{
		GLSL::disableVertexAttribArray(h_tex);
	}
	if (h_nor != -1)
	{
		GLSL::disableVertexAttribArray(h_nor);
	}
	GLSL::disableVertexAttribArray(h_pos);
	CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	CHECKED_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
