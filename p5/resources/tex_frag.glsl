#version 330 core
uniform sampler2D Texture0;

in vec2 vTexCoord;
in float dCo;

// Color
in vec3 fragNor;
out vec4 color;

// Material
uniform vec3 difCol;
uniform vec3 matAmb;
uniform vec3 matSpec;
uniform vec3 lightCol;
uniform float shine;

// Textures

in vec3 lightDir;
in vec3 view;

void main() {
	vec4 texColor0 = texture(Texture0, vTexCoord);
  
	vec3 normal = normalize(fragNor);
	vec3 light = normalize(lightDir);
	vec3 ambient = vec3(matAmb * lightCol);
	vec3 difColor = vec3(texColor0.xyz *(max(0, dot(normal, light))));

	vec3 h = (light + view) / length(light + view);
	vec3 specular = vec3(matSpec * pow(dot(normal, h), shine) * lightCol);
	color = vec4(difColor + specular + ambient, 1);
}

