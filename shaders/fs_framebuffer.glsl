#version 330 core
in vec2 tex;

void main()
{
	//vec4 color = texture(depth, tex_coord);
	gl_FragColor = vec4(tex.x, tex.y , 1.0, 1.0);
}
