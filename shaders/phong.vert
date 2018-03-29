#version 330 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex_coord;

uniform mat4 MVP;

out vec3 vVertexPosition;
out vec3 vNormal;
out vec2 vTexCoord;

void main()
{
	vec4 pos = MVP * vec4(vertex_position, 1.0);
	vVertexPosition = vertex_position;
	vNormal = normal;
	gl_Position=vec4(vertex_position, 1.0);
}
