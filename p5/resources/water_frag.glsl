#version 330 core 
flat in vec3 fragNor;
out vec4 color;

uniform float time;

uniform vec3 difCol;
uniform vec3 matAmb;
uniform vec3 matSpec;
uniform vec3 lightCol;
uniform float shine;

in vec3 lightDir;
in vec3 view;


void main()
{
	vec3 normal = normalize(fragNor);
	// Map normal in the range [-1, 1] to color in range [0, 1];
	vec3 Ncolor = 0.5*normal + 0.2;

	Ncolor.x *= 3 * sin(time);
	//Ncolor.z *= sin(time);
	color = vec4(Ncolor + 0.2, 1.0);

	vec3 light = normalize(lightDir);
	vec3 ambient = vec3(matAmb * lightCol);
	vec3 difColor = vec3(difCol*(max(0, dot(normal, light))));

	vec3 h = (light + view) / length(light + view);
	vec3 specular = vec3(matSpec * pow(dot(normal, h), shine) * lightCol);
	color = vec4(difColor + specular + ambient, 1);
	color.a = 0.6;
}
