#version 330 core

uniform sampler2D uDepth;
uniform sampler2D uAlbedo;
uniform sampler2D uNormals;

in vec2 v_texCoord;
out vec3 v_outColor;

vec3 light_pos = vec3(0.0, 0.0, 0.0);

vec3 GGX()
{
	return vec3(1.0);
}

vec3 Fresnel()
{
	return vec3(1.0);	
}

vec3 Geometric()
{
	return vec3(1.0);
}

void main()
{
	vec3 position = texture(uDepth, v_texCoord).xyz;
	vec3 albedo   = texture(uAlbedo, v_texCoord).xyz;
	vec3 normal   = texture(uNormals, v_texCoord).xyz;
	
	vec3 N = normalize(normal);
	vec3 L = normalize(light_pos - position);
	float NdotL = dot(L, N);

	v_outColor = albedo * NdotL;

}
