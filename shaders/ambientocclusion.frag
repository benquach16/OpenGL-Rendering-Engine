#version 330 core

uniform sampler2D uTexture;
uniform sampler2D uDepth;
uniform sampler2D uTexNoise;
uniform vec3 samples[64];

in vec2 v_texCoord;

out vec4 v_outColor;

void main()
{
	vec4 color = texture(uTexture, v_texCoord);

	float currentDepth = texture(uDepth, v_texCoord).x;
	vec4 sample = texture(uDepth, v_texCoord+vec2(0.0, 0.001));
	vec4 sample2 = texture(uDepth, v_texCoord-vec2(0.0, 0.1));
	vec4 sample3 = texture(uDepth, v_texCoord+vec2(0.1, 0.0));
	vec4 sample4 = texture(uDepth, v_texCoord-vec2(0.1, 0.0));	
	color += (sample.x >= currentDepth + 0.025 ? 1.0 : 0.0);
	v_outColor = color;
}
