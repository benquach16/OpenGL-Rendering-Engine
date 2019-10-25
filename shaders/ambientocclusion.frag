#version 330 core

uniform sampler2D uTexture;
uniform sampler2D uDepth;

in vec2 v_texCoord;

out vec4 v_outColor;

void main()
{
	vec4 color = texture(uTexture, v_texCoord);
	color -= pow(texture(uDepth, v_texCoord).x, 32.0)/6;
	v_outColor = color;
}
