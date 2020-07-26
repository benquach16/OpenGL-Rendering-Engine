#version 330 core

uniform sampler2D uTexture;
uniform sampler2D uBloom;
in vec2 v_texCoord;

out vec4 v_outColor;

void main()
{
    const float exposure = 1.5;
    const float gamma = 2.2;
    vec3 color = texture(uTexture, v_texCoord).xyz;
    vec3 bloom = texture(uBloom, v_texCoord).xyz;
    
    color = pow(color, vec3(gamma));
    color += bloom * 0.5;
	float luma = dot(color, vec3(0.2126, 0.7152, 0.0722));
	float toneMappedLuma = luma / (1. + luma);
	color *= toneMappedLuma / luma;
	color = pow(color, vec3(1. / gamma));

    v_outColor = vec4(color, 1.0);
}
