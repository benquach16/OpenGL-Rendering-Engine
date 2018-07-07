#version 330 core

uniform sampler2D uDepth;
uniform sampler2D uAlbedo;
uniform sampler2D uNormals;

in vec2 v_texCoord;

vec3 GGX()
{
}

void main()
{
	vec3 position = texture(uDepth, v_texCoord);
	vec3 albedo   = texture(uAlbedo, v_texCoord);
	vec3 normal   = texture(uNormals, v_texCoord);

	
}
