#version 330 core
in vec3 vVertexPosition;
in vec3 vNormal;
in vec2 vTexCoord;

layout(location = 0) out vec3 out_Position;
layout(location = 1) out vec3 out_Color;
layout(location = 2) out vec3 out_Normal;

vec3 light_pos = vec3(0.5, 0.3, -1.5);
void main()
{
	vec4 color = vec4(0.0, 1.0, 1.0, 1.0);

	out_Position = vVertexPosition;
	out_Color = color.xyz;
	out_Normal = normalize(vNormal);

}
