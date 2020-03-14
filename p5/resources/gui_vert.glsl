#version  330 core
layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec3 vertNor;
layout(location = 2) in vec2 vertTex;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

in vec2 position;
out vec2 vTexCoord;

out vec2 textureCoords;
uniform mat4 transformationMatrix;

void main()
{
	vTexCoord = vertTex;
	vec4 newPos =  transformationMatrix * vec4(vertPos.x, vertPos.y, 0.0, 1.0);

	gl_Position =  newPos;
	textureCoords = vec2((vertPos.x+1.0)/2.0, 1 - (vertPos.y+1.0)/2.0);
}
