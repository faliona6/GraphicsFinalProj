#include "Transform.h"
#include "MatrixStack.h"

using namespace glm;

Transform::Transform()
{
	position = vec3(0, 0, 0);
	scale = vec3(1, 1, 1);
	rotation = vec3(0, 0, 0);
}

Transform::Transform(vec3 position)
{
	this->position = position;
	scale = vec3(1, 1, 1);
	rotation = vec3(0, 0, 0);
}


Transform::~Transform()
{
}

vec3 Transform::getForward()
{
	vec4 forward = vec4(0, 0, 1, 1);
	shared_ptr<MatrixStack> Model = make_shared<MatrixStack>();
	Model->pushMatrix();
	Model->rotate(rotation.x, vec3(1, 0, 0));
	Model->rotate(rotation.y, vec3(0, 1, 0));
	Model->rotate(rotation.z, vec3(0, 0, 1));
	forward = Model->topMatrix() * forward;
	Model->popMatrix();
	return normalize(vec3(forward));
}

