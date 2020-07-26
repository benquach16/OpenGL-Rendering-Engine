#version 330 core

uniform sampler2D uTexture;
uniform sampler2D uBloom;
in vec2 v_texCoord;

out vec4 v_outColor;

void main()
{
    const float exposure = 1.5;
    const float gamma = 2.2;
    vec3 hdrColor = texture(uTexture, v_texCoord).xyz;
    vec3 bloom = texture(uBloom, v_texCoord).xyz;
    hdrColor += bloom * 0.5;
    //hdrColor = vec3(1.0) - exp(-hdrColor * exposure);
    //hdrColor = pow(hdrColor, vec3(1.0/gamma));
    v_outColor = vec4(hdrColor, 1.0);
}
