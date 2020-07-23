#version 330 core

uniform sampler2D uTexture;
uniform sampler2D uDepth;
uniform sampler2D uNormal;
uniform sampler2D uTexNoise;
uniform vec3 samples[64];

uniform mat4 projection;

in vec2 v_texCoord;

out vec4 v_outColor;

vec4 proj_to_screen(vec4 proj)
{
	vec4 screen = proj;
	screen.x = (proj.x + proj.w);
	screen.y = (proj.w - proj.y);
	screen.xy *= 0.5;
	return screen;
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
	v_outColor = vec4(occlusion);
    //v_outColor = vec4(color*occlusion, 1.0);
}
