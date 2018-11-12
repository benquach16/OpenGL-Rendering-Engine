#version 330 core

uniform sampler2D uDepth;
uniform sampler2D uPosition;
uniform sampler2D uAlbedo;
uniform sampler2D uNormals;
uniform vec3 uCameraPosition;

in vec2 v_texCoord;
out vec3 v_outColor;

const float PI = 3.14151;


//transform me into screen space!
vec3 light_pos = vec3(2.0, 2.0, 3.0);
vec3 camera_pos = vec3(0.0, 0.0, 2.0);

float roughness = 0.6;


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

float chi(float v)
{
	return v > 0 ? 1 : 0;
}

float GGX(vec3 N, vec3 H)
{
	float alpha = 0.6;
    float NoH = dot(N,H);
	float alpha2 = alpha * alpha;
	float NoH2 = NoH * NoH;
	float den = NoH2 * alpha2 + (1 - NoH2);
	return (alpha2) / ( PI * den * den );
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
	vec3 L = -normalize(position - light_pos);
	vec3 R = normalize(reflect(L,N));
	vec3 H = normalize(-L + V);

	float NdotL = max(dot(L, N), 0.0);
	float VdotN = max(dot(V, N), 0.0);
	float spec = dot(V, R);

	vec3 numerator = GGX(N, H) * Fresnel() * Geometric();
	vec3 denominator;
	
	//do lighting once we transform color space
	albedo = toGammaSpace(albedo);
	//albedo = albedo * NdotL + max(pow(spec, 50.0), 0.0);
	albedo  = albedo * GGX(N,H);
	albedo = toLinearSpace(albedo);
	v_outColor = albedo;

}
