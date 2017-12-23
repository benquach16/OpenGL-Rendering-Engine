uniform sampler2D depth;
uniform sampler2D albedo;

varying vec2 texCoords;

void main()
{
	vec4 color = texture(depth, texCoords);
	gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
