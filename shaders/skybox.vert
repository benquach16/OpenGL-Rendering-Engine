#version 330 core
layout (location = 0) in vec3 a_vertexPosition;

out vec3 v_texCoords;

//uniform mat4 projection;
//uniform mat4 view;
uniform mat4 MVP;

void main()
{
	v_texCoords = a_vertexPosition;
	vec4 pos = MVP*vec4(a_vertexPosition, 1.0);
	gl_Position = pos.xyww;
}  
