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
	vec4 pos = MVP*vec4(vertex_position, 1.0);
	vVertexPosition = pos.xyz;
	vNormal = normalize(normal);
	vTexCoord = tex_coord;
	//gl_Position=vec4(vVertexPosition,1.0);
	gl_Position = pos;
}
