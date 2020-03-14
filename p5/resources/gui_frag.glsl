#version 330 core

in vec2 vTexCoord;
uniform sampler2D Texture0;

out vec4 out_Color;

//uniform sampler2D guiTexture;

void main(void){

	//out_Color = texture(guiTexture,textureCoords);
	vec4 texColor0 = texture(Texture0, vTexCoord);
	if(length(texColor0.xyz) < 0.2)
		discard;
	out_Color = texColor0;

}