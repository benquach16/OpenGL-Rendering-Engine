#version 330 core
uniform sampler2D depth;

in vec2 v_texCoord;

void main()
{
	vec4 color = texture(depth, v_texCoord);
	gl_FragColor = vec4(v_texCoord.x, v_texCoord.y , 1.0, 1.0);
	//gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
	//gl_FragColor=color;
}
