#version 330 core

in vec3 vertex_position;
in vec3 normal;
in vec2 tex_coord;

layout(location = 0) out vec3 vColor;

vec3 light_pos = vec3(-1.0, 1.0, 0.0);

void main()
{
	vColor=normal.xyz;
}
