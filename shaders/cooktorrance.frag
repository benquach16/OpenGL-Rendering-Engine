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

float roughness = 0.3;
float metalness = 0.9;
float ior = 1.4;

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

float GGX_NormalDistribution(vec3 N, vec3 H)
{
    float NoH = dot(N,H);
	float alpha2 = roughness * roughness;
	float NoH2 = NoH * NoH;
	float den = NoH2 * alpha2 + (1 - NoH2);
	return (alpha2) / ( PI * den * den );
}

vec3 Fresnel(float cosT, vec3 F0)
{
	return F0 + (1.0-F0) * pow(1.0 - cosT, 5.0);
}

float GGX_Geometric(vec3 N, vec3 H, vec3 V)
{
	float alpha = roughness;
	float VoH2 = max(dot(V, H), 0.0);
    float chi = chi( VoH2 / max(dot(V,N), 0.0));
	VoH2 = VoH2 * VoH2;
	float tan2 = ( 1 - VoH2 ) / VoH2;
	return (chi * 2) / ( 1 + sqrt( 1 + alpha * alpha * tan2 ) );
}



void main()
{
	vec3 position = texture(uDepth, v_texCoord).xyz;
	vec3 albedo   = texture(uAlbedo, v_texCoord).xyz;
	vec3 normal   = texture(uNormals, v_texCoord).xyz;

	vec3 V = -normalize(position - camera_pos);
	vec3 N = normalize(normal);
	vec3 L = normalize(light_pos - position);
	vec3 R = normalize(-reflect(L,N));
	vec3 H = normalize(L + V);

	float NdotL = max(dot(L, N), 0.0);
	float VdotN = max(dot(V, N), 0.0);
	float VdotH = max(dot(V, H), 0.0);
	float spec = dot(V, R);

	vec3 F0 = vec3(abs((1.0 - ior) / (1.0 + ior)));
	F0 = F0 * F0;
	F0 = mix(F0, albedo, metalness);
	

	vec3 denominator;
	
	//do lighting once we transform color space
	albedo = toGammaSpace(albedo);
	//albedo = albedo * NdotL + max(pow(spec, 50.0), 0.0);
	//albedo = albedo * GGX_NormalDistribution(N,H) * GGX_Geometric(N, H, V);
	float D = GGX_NormalDistribution(N, H);
	float G = GGX_Geometric(N, H, V);
	float F = pow(1.0 - VdotN, 6.0);

	float Rs = G * F * D / max(3.14159265 * VdotN * NdotL, 0.000001);
	albedo = albedo * NdotL + vec3(1.0) * NdotL * (roughness + Rs * (1.0 - roughness));
	albedo = toLinearSpace(albedo);
	v_outColor = albedo;

}
