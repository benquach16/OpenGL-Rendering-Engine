#version 330 core

uniform sampler2D uDepth;
uniform sampler2D uPosition;
uniform sampler2D uAlbedo;
uniform sampler2D uNormals;
uniform vec3 uCameraPosition;

in vec2 v_texCoord;
out vec3 v_outColor;


//transform me into screen space!
vec3 light_pos = vec3(-0.5, 0.5, -0.6);
vec3 camera_pos = vec3(0.5f, 0.5f, 1.5f);

float roughness = 0.2;


vec3 toGammaSpace(vec3 color)
{
	color.x = pow(color.x, 2.2);
	color.y = pow(color.y, 2.2);
	color.z = pow(color.z, 2.2);
	return color;
}

vec3 toLinearSpace(vec3 color)
{
	color.x = pow(color.x, 1.0/2.2);
	color.y = pow(color.y, 1.0/2.2);
	color.z = pow(color.z, 1.0/2.2);
	return color;
}


vec3 GGX()
{
	float alpha2 = roughness * roughness;
	
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

	vec3 V = normalize(position - camera_pos);
	vec3 N = normalize(normal);
	vec3 L = normalize(position - light_pos);
	vec3 R = normalize(reflect(L,N));

	float NdotL = max(dot(L, N), 0.0);
	float VdotN = max(dot(V, N), 0.0);
	float spec = dot(V, R);

	//do lighting once we transform color space
	albedo = toGammaSpace(albedo);
	albedo = albedo * NdotL + max(0.0, pow(spec, 5.0));
	
	albedo = toLinearSpace(albedo);
	v_outColor = albedo;

}
