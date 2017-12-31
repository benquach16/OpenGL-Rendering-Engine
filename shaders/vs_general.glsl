#version 330 core
//attribute vec3 vertex_position;
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 tex_coord;

void main()
{
	gl_Position=vec4(vertex_position, 1.0);
}
