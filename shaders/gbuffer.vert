#version 330 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex_coord;

uniform mat4 MVP;
uniform mat4 view;

out vec3 v_outNormal;
out vec2 v_outTexCoord;

void main()
{
	vec4 pos_mvp = MVP*vec4(vertex_position, 1.0);
	//vec4 norm = view*vec4(normal, 1.0);
	v_outNormal = normalize(normal);
	v_outTexCoord = tex_coord;
	//gl_Position=vec4(vVertexPosition,1.0);
	gl_Position = pos_mvp;
}
