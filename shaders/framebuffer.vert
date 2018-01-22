#version 330 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex_coord;

out vec2 out_tex_coord;

void main()
{
	out_tex_coord = tex_coord;
	gl_Position=vec4(vertex_position, 1.0);
}
