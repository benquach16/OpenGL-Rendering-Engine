#version 330 core

uniform mat4 MVP;

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex_coord;

out vec3 out_vertex_position;
out vec3 out_normal;
out vec2 out_tex_coord;

void main()
{
	out_vertex_position = vertex_position;
	out_normal = normal;
	gl_Position=vec4(vertex_position, 1.0);
}
