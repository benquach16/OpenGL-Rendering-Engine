#version 330 core

uniform sampler2D uOcclusion;

in vec2 v_texCoord;

out vec4 v_outColor;

void main()
{
    vec2 texelSize = 1.0 / vec2(textureSize(uOcclusion, 0));
    float result = 0.0;
    for (int x = -2; x < 2; ++x) 
    {
        for (int y = -2; y < 2; ++y) 
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(uOcclusion, v_texCoord + offset).r;
        }
    }
    result = result / (4.0 * 4.0);

    v_outColor = vec4(result);
}
