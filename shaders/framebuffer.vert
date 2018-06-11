#version 330 core

layout(location = 0) in vec3 a_vertexPosition;
layout(location = 1) in vec2 a_texCoord;

out vec2 v_texCoord;

void main()
{
	v_texCoord = a_texCoord;
	gl_Position=vec4(a_vertexPosition, 1.0);
}
