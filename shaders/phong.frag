#version 330 core
in vec3 vVertexPosition;
in vec3 vNormal;
in vec2 vTexCoord;

layout(location = 0) out vec3 out_Color;
layout(location = 1) out vec3 out_Normal;

vec3 light_pos = vec3(0.0, 0.0, -1.0);
void main()
{
	vec3 N = -normalize(vNormal);
	vec4 color = vec4(0.0, 1.0, 1.0, 1.0);
	vec3 L = normalize(light_pos - vVertexPosition);
	float LN = dot(L, N);
	color = color * LN;
	out_Color = color.xyz;
	out_Normal = N;
	//vColor=color.xyz;
	//gl_FragColor=vec4(vNormal, 1.0);
}
