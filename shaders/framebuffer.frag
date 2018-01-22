#version 330 core
uniform sampler2D depth;
in vec2 tex_coord;

void main()
{
	vec4 color = texture(depth, tex_coord);
	gl_FragColor = vec4(tex_coord.x, tex_coord.y , 1.0, 1.0);
	//gl_FragColor=color;
}
