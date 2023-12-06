#version 460 core

layout (location = 0) in vec4 in_position;
layout (location = 1) in vec2 in_uv;
layout (location = 2) in vec3 in_normal;

layout(location = 0) out vec4 out_GBuffer0;
layout(location = 1) out vec4 out_GBuffer1;
layout(location = 2) out vec4 out_GBuffer2;
layout(location = 3) out vec4 out_GBuffer3;
layout(location = 4) out vec4 out_GBuffer4;

struct Material {
	float hasDiffuseTexture;
	vec3 diffuseColor;
	float hasNormalTexture;
	float hasMetallicTexture;
	float metallic;
	float hasRoughnessTexture;
	float roughness;
	float hasAOTexture;
	float ao;
	float emissive;
};

// use storage buffer?
// a temp buffer struct for what data in material
layout (std430, binding = 1) buffer material {
	vec4 data[3];
}materialData;

layout (location = 2) uniform sampler2D u_blendMap;

uniform sampler2D u_diffuseTextures[4];
//uniform sampler2D u_normalTexture[4];

void main() {

	// blendMap config
	vec4 blendMapColor = texture(u_blendMap, in_uv);
	vec2 tiledCoords = in_uv * 40.0;
	
	float bgAmount = 1.0 - (blendMapColor.r + blendMapColor.g + blendMapColor.b);

	// normal
	vec3 fragNormal = in_normal;
	
	float metallic = 0.0;
	float roughness = 0.0;
	float ao = 1.0;
	float emissive = 2.0;
	
	// diffuse color
	vec4 bgColor = texture(u_diffuseTextures[3], tiledCoords) * bgAmount;
	vec4 rColor = texture(u_diffuseTextures[0], tiledCoords) * blendMapColor.r;
	vec4 gColor = texture(u_diffuseTextures[1], tiledCoords) * blendMapColor.g;
	vec4 bColor = texture(u_diffuseTextures[2], tiledCoords) * blendMapColor.b;
	
	vec3 diffuseColor = bgColor.rgb + rColor.rgb + gColor.rgb + bColor.rgb;
	
	out_GBuffer0 = vec4(in_position.xyz, fragNormal.x);
	out_GBuffer1 = vec4(fragNormal.yz, diffuseColor.xy);
	out_GBuffer2 = vec4(diffuseColor.z, metallic, roughness, ao);
	out_GBuffer3 = vec4(emissive, 0, 0, 0);
	
}



