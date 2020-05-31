#version 330 core
out vec4 v_outColor;

in vec3 v_texCoords;

uniform samplerCube uSkybox;

void main()
{
	v_outColor = texture(uSkybox, v_texCoords).xyzw;
	//FragColor = vec3(1.0, 1.0, 1.0);
}
