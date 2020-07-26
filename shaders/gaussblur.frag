#version 330 core

uniform sampler2D uTexture;
uniform vec2 uDirection;
vec2 resolution = vec2(1600, 900);

in vec2 v_texCoord;

out vec4 v_outColor;

void main() {
  vec4 color = vec4(0.0);
  vec2 off1 = vec2(1.411764705882353) * uDirection;
  vec2 off2 = vec2(3.2941176470588234) * uDirection;
  vec2 off3 = vec2(5.176470588235294) * uDirection;
  color += texture(uTexture, v_texCoord) * 0.1964825501511404;
  color += texture(uTexture, v_texCoord + (off1 / resolution)) * 0.2969069646728344;
  color += texture(uTexture, v_texCoord - (off1 / resolution)) * 0.2969069646728344;
  color += texture(uTexture, v_texCoord + (off2 / resolution)) * 0.09447039785044732;
  color += texture(uTexture, v_texCoord - (off2 / resolution)) * 0.09447039785044732;
  color += texture(uTexture, v_texCoord + (off3 / resolution)) * 0.010381362401148057;
  color += texture(uTexture, v_texCoord - (off3 / resolution)) * 0.010381362401148057;
  v_outColor = color;
}
