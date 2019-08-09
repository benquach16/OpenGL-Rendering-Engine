#version 330 core

uniform sampler2D uTexture;

in vec2 v_texCoord;

out vec4 v_outColor;

void main()
{
	vec4 color = texture(uTexture, v_texCoord);
	v_outColor = color;
}
