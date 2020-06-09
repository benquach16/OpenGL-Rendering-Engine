#version 330 core

uniform sampler2D uTexture;
uniform sampler2D uDepth;
uniform sampler2D uNormal;
uniform sampler2D uTexNoise;
uniform vec3 samples[64];

uniform mat4 projMatrixInv;
uniform mat4 viewMatrixInv;

in vec2 v_texCoord;

out vec4 v_outColor;

vec3 WorldPosFromDepth(float depth) {
    float z = depth * 2.0 - 1.0;

    vec4 clipSpacePosition = vec4(v_texCoord * 2.0 - 1.0, z, 1.0);
    vec4 viewSpacePosition = projMatrixInv * clipSpacePosition;

    // Perspective division
    viewSpacePosition /= viewSpacePosition.w;

    vec4 worldSpacePosition = viewMatrixInv * viewSpacePosition;

    return worldSpacePosition.xyz;
}

void main()
{
	vec4 color = texture(uTexture, v_texCoord);

	float currentDepth = texture(uDepth, v_texCoord).x;
	vec3 position = WorldPosFromDepth(currentDepth);
	vec3 normal = texture(uNormal, v_texCoord).xyz;
	vec3 randomVec = texture(uTexNoise, v_texCoord * 200).xyz;

	vec3 tangent   = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN       = mat3(tangent, bitangent, normal); 

	float occlusion = 0.0;
	float radius = 0.5;
	for(int i = 0; i < 64; ++i)
	{
		// get sample position
		vec3 sample = TBN * samples[i]; // from tangent to view-space
		sample = position + sample * radius; 
	}  

	color = vec4(position,1.0);

	v_outColor = color;
}
