#version 330 core

in vec3 vertex_position;
in vec3 normal;
in vec2 tex_coord;

vec3 light_pos = vec3(-1.0, 1.0, 0.0);

void main()
{
	vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
	vec3 L = light_pos - vertex_position;
	float LN = dot(L, normal);
	color = color * LN;
	gl_FragColor=color;
}
