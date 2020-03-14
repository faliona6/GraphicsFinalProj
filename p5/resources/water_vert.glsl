#version  330 core
layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec3 vertNor;
layout(location = 2) in vec4 indicators;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

uniform vec3 eyePos;

uniform float time;

flat out vec3 fragNor;
const float PI = 3.1415926535897932384626433832795;

const float waveLength = 3.0;
const float waveAmplitude = 0.3;
const float shineDamper = 20.0;

uniform vec3 lightPos;

out vec3 lightDir;
out vec3 view;

float generateOffset(float x, float z, float val1, float val2){
	float radiansX = ((mod(x+z*x*val1, waveLength)/waveLength) + (time * 0.2) * mod(x * 0.8 + z, 1.5)) * 2.0 * PI;
	float radiansZ = ((mod(val2 * (z*x +x*z), waveLength)/waveLength) + (time * 0.2)* 2.0 * mod(x , 2.0) ) * 2.0 * PI;
	return waveAmplitude * 0.5 * (sin(radiansZ) + cos(radiansX));
}

vec3 applyDistortion(vec3 vertex){
	float xDistortion = generateOffset(vertex.x, vertex.z, 0.2, 0.1);
	float yDistortion = generateOffset(vertex.x, vertex.z, 0.1, 0.3);
	float zDistortion = generateOffset(vertex.x, vertex.z, 0.15, 0.2);
	return vertex + vec3(xDistortion, yDistortion, zDistortion);
}

vec3 calcNormal(vec3 vertex0, vec3 vertex1, vec3 vertex2){
	vec3 tangent = vertex1 - vertex0;
	vec3 bitangent = vertex2 - vertex0;
	return normalize(cross(tangent, bitangent));
}

void main()
{	
	vec3 currentVertex = vertPos.xyz;
	vec3 vertex1 = currentVertex + vec3(indicators.x, 0.0, indicators.y);
	vec3 vertex2 = currentVertex + vec3(indicators.z, 0.0, indicators.w);

	//apply distortion to all 3 vertices
	currentVertex = applyDistortion(currentVertex);
	vertex1 = applyDistortion(vertex1);
	vertex2 = applyDistortion(vertex2);

	fragNor = calcNormal(currentVertex, vertex1, vertex2);

	vec4 curVertPos = vec4(currentVertex.xyz, 1.0);

	gl_Position = P * V * M * curVertPos;
	
	//fragNor = (M * vec4(vertNor, 0.0)).xyz;
	//vec3 dist = applyDistortion(vertNor);
	//fragNor = (M * vec4(dist, 0.0)).xyz;
	view = eyePos - (M * curVertPos).xyz;
	//fragNor = (M * vec4(vertNor, 0.0)).xyz;

	lightDir = lightPos - (M*curVertPos).xyz;
}
