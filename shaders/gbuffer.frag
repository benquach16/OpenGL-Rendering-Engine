#version 330 core
in vec3 vVertexPosition;
in vec3 vNormal;
in vec2 vTexCoord;

layout(location = 0) out vec3 oAlbedo;
layout(location = 1) out vec3 oNormal;


void main()
{
	oColor = vNormal;
	oNormal = vNormal;
}
