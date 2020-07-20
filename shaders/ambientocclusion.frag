#version 330 core

uniform sampler2D uTexture;
uniform sampler2D uDepth;
uniform sampler2D uNormal;
uniform sampler2D uTexNoise;
uniform vec3 samples[64];

uniform mat4 projection;

in vec2 v_texCoord;

out vec4 v_outColor;

vec3 arr[32] = vec3[32](
        vec3(-0.134, 0.044, -0.825),
        vec3(0.045, -0.431, -0.529),
        vec3(-0.537, 0.195, -0.371),
        vec3(0.525, -0.397, 0.713),
        vec3(0.895, 0.302, 0.139),
        vec3(-0.613, -0.408, -0.141),
        vec3(0.307, 0.822, 0.169),
        vec3(-0.819, 0.037, -0.388),
        vec3(0.376, 0.009, 0.193),
        vec3(-0.006, -0.103, -0.035),
        vec3(0.098, 0.393, 0.019),
        vec3(0.542, -0.218, -0.593),
        vec3(0.526, -0.183, 0.424),
        vec3(-0.529, -0.178, 0.684),
        vec3(0.066, -0.657, -0.570),
        vec3(-0.214, 0.288, 0.188),
        vec3(-0.689, -0.222, -0.192),
        vec3(-0.008, -0.212, -0.721),
        vec3(0.053, -0.863, 0.054),
        vec3(0.639, -0.558, 0.289),
        vec3(-0.255, 0.958, 0.099),
        vec3(-0.488, 0.473, -0.381),
        vec3(-0.592, -0.332, 0.137),
        vec3(0.080, 0.756, -0.494),
        vec3(-0.638, 0.319, 0.686),
        vec3(-0.663, 0.230, -0.634),
        vec3(0.235, -0.547, 0.664),
        vec3(0.164, -0.710, 0.086),
        vec3(-0.009, 0.493, -0.038),
        vec3(-0.322, 0.147, -0.105),
        vec3(-0.554, -0.725, 0.289),
        vec3(0.534, 0.157, -0.250)
);

vec4 screen_to_proj(vec2 screen, float z)
{
	vec4 proj;
	proj.w = 1.0;
	proj.z = z;
	proj.x = screen.x*2 - proj.w;
	proj.y = -screen.y*2 + proj.w;
	return proj;
}


vec4 convert_to_screen_space(vec4 proj)
{
	vec4 screen;
	screen.x = (proj.x + proj.w)*0.5;
	screen.y = (proj.w - proj.y)*0.5;
	screen.z = proj.z;
	screen.w = proj.w;
	return screen;
}

vec4 proj_to_screen(vec4 proj)
{
	vec4 screen = proj;
	screen.x = (proj.x + proj.w);
	screen.y = (proj.w - proj.y);
	screen.xy *= 0.5;
	return screen;
}

float LinearizeDepth(float depth)
{
	float zNear = 0.1f;
	float zFar = 200.0f;
    float z_n = 2.0 * depth - 1.0;
    return -(2.0 * zNear * zFar) / (zFar + zNear - z_n * (zFar - zNear));
}

vec3 posFromDepth(in vec2 Tex, in float d){ // get eye coordinate from depth
    vec3 pos = vec3(Tex, d); 

    // transform by the projection inverse 
    vec4 clip = inverse(projection)*vec4(pos*2-1,1);

    // divide by w to get the position. 
    return clip.xyz/clip.w;
}
const vec2 noiseScale = vec2(1600.0/4.0, 900.0/4.0); // screen = 800x600
void main()
{
	vec3 color = texture(uTexture, v_texCoord).xyz;
    float d = texture(uDepth, v_texCoord).x;
    
	vec3 position = posFromDepth(v_texCoord, d);
    //vec3 position = texture(uDepth, v_texCoord).xyz;
    vec3 normal = texture(uNormal, v_texCoord).xyz;
    vec3 randomVec = texture(uTexNoise, v_texCoord * noiseScale).xyz; 

    vec3 tangent   = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN       = mat3(tangent, bitangent, normal); 
        
    float occlusion = 0.0;
    float radius = 5.0;
    //float scale = radius / position.z;
    float scale =0.05;
    float falloff = 1.0/(radius * radius);
    int cSamples = 64;
    //vec3 samplePosView = position + ((arr[0] + normal) * scale);
    
    for(int i = 0; i < cSamples; ++i)
    {
		//vec3 samplePosView = position + ((arr[i] + normal) * scale);
        vec3 samplePosView = TBN * samples[i];
        samplePosView = position + samplePosView * radius;
		vec4 samplePosScreen = projection * vec4(samplePosView, 1.0);
        //samplePosScreen = proj_to_screen(samplePosScreen);
		samplePosScreen.xy /= samplePosScreen.w;
        samplePosScreen.xy = samplePosScreen.xy * 0.5 + vec2(0.5);
        
        vec3 sample = posFromDepth(v_texCoord, texture(uDepth, samplePosScreen.xy).x);
        //vec3 sample = texture(uDepth, samplePosScreen.xy).xyz;
        vec3 V = sample - position;
        float VdotV = dot(V, V);
        float VdotN = dot(V, normal);

		float occ_coeff = clamp(1.0 - falloff * VdotV, 0.0, 1.0);
        occ_coeff *= clamp(VdotN * inversesqrt(VdotV), 0.0, 1.0);
        occ_coeff = clamp(occ_coeff, 0.0, 1.0);
        //occlusion += occ_coeff;
        
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(position.z - sample.z));
        occlusion       += (sample.z >= position.z + 0.025 ? 1.0 : 0.0) * rangeCheck;   
    } 
    
    occlusion = (occlusion/cSamples);
    occlusion = 1.0 - occlusion;
    //occlusion = clamp(occlusion, 0.0, 1.0);
    //occlusion = 1 - 1.2 + (1.2 * occlusion);
	//v_outColor = vec4(occlusion);
    v_outColor = vec4(color*occlusion, 1.0);
}
