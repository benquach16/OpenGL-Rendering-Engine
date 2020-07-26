#version 330 core

uniform sampler2D uPosition;
uniform sampler2D uAlbedo;
uniform sampler2D uNormals;
uniform sampler2D uOcclusion;
uniform samplerCube uCubemap;
uniform vec3 uCameraPosition;
uniform mat4 uMVP;

in vec2 v_texCoord;
out vec3 v_outColor;

const float PI = 3.14151;


//transform me into screen space!
vec3 light_pos = vec3(2.0, 2.0, 3.0);
vec3 camera_pos = vec3(0.0, 0.0, 2.0);

float roughness = 0.02;
float metalness = 0.1;
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

float GGX_NormalDistribution(float NdotH)
{
	float alpha2 = roughness * roughness;
	float NoH2 = NdotH * NdotH;
	float den = NoH2 * alpha2 + (1 - NoH2);
	return (alpha2) / ( PI * den * den );
}

vec3 Fresnel(float cosT, vec3 F0)
{
	return F0 + (1.0-F0) * pow(1.0 - cosT, 5.0);
}

float GGX_Geometric(float NdotL, float VdotN)
{
	float r2 = roughness * roughness;
	float gv = NdotL * sqrt( VdotN * ( VdotN - VdotN * r2 ) + r2 );
	float gl = VdotN * sqrt( NdotL * ( NdotL - NdotL * r2 ) + r2 );
	return 0.5 / max( gv + gl, 0.00001 );
}

void main()
{
	vec3 position = texture(uPosition, v_texCoord).xyz;
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
	float NdotH = max(dot(N, H), 0.0);
	float LdotH = max(dot(L, H), 0.0);

	float spec = dot(V, R);

	vec3 F0 = vec3(abs((1.0 - ior) / (1.0 + ior)));
	F0 = F0 * F0;
	F0 = mix(F0, albedo, metalness);
	
	//do lighting once we transform color space
	albedo = toGammaSpace(albedo);
	//albedo = albedo * NdotL + max(pow(spec, 50.0), 0.0);
	//albedo = albedo * GGX_NormalDistribution(N,H) * GGX_Geometric(N, H, V);
	float D = GGX_NormalDistribution(NdotH);
	float G = GGX_Geometric(NdotL, VdotN);
	vec3 F = Fresnel(VdotN, F0);
	
	vec3 Rs = G * F * D * PI * NdotL;
	//albedo = albedo * NdotL + vec3(1.0) * NdotL * (roughness + Rs * (1.0 - roughness));
	//vec3 indirect = vec3(0.2);
	//indirect * 0.05;
	vec3 indirect = texture(uCubemap, R).xyz;

	Rs += (indirect * F) * 0.1;

	//Rs += F;

	vec3 diffuse = albedo * NdotL * 1.0/PI;

	albedo = diffuse + Rs;
	//albedo = albedo * 2.0 * NdotL;

	albedo = toLinearSpace(albedo);
	vec3 occlusion = texture(uOcclusion, v_texCoord).xyz;
	occlusion = 1 - 1.2 + (occlusion * 1.2);
	albedo = albedo * occlusion;

	v_outColor = albedo;
}
