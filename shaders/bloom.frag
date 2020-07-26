#version 330 core

uniform sampler2D uTexture;

in vec2 v_texCoord;

out vec4 v_outColor;

void main()
{
    vec3 color = texture(uTexture, v_texCoord).rgb;
    float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));
    color = color * step(0.8, brightness);
    v_outColor = vec4(color, 1.0);
}
