#version 330 core

uniform sampler2D uTexture;

in vec2 v_texCoord;

void main()
{
	vec4 color = texture(uTexture, v_texCoord);
	gl_FragColor = color;
}
