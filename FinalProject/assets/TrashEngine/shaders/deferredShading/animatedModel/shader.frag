#version 460 core

// output 
in vec3 fragPos;
in vec2 texCoord;
layout (location = 2) in vec3 vs_normal;		// normal in world space
layout (location = 3) in vec3 vs_tangent;		// tangent in world space
layout (location = 4) in vec3 vs_bitangent;	// bitangent in world space

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

layout(location = 0) uniform sampler2D diffuseSampler;
layout(location = 1) uniform sampler2D normalTextureSampler;

void main() {

	Material material;
	material.hasDiffuseTexture = materialData.data[0].x;
	material.diffuseColor = materialData.data[0].yzw;
	material.hasNormalTexture = materialData.data[1].x;
	material.hasMetallicTexture = materialData.data[1].y;
	material.metallic = materialData.data[1].z;
	material.hasRoughnessTexture = materialData.data[1].w;
	material.roughness = materialData.data[2].x;
	material.hasAOTexture = materialData.data[2].y;
	material.ao = materialData.data[2].z;
	material.emissive = materialData.data[2].w;

	vec3 diffuseColor;

	if (material.hasDiffuseTexture > 0) {
		diffuseColor = texture(diffuseSampler, texCoord).rgb;
	} else {
		diffuseColor = material.diffuseColor;
	}
	//diffuseColor = materialDiffuseColor;
	
	vec3 fragNormal;
	if (material.hasNormalTexture > 0) {
		vec3 normalMapColor = texture(normalTextureSampler, texCoord).rgb * 2.0 - 1.0;
		fragNormal = vec3(normalMapColor.r * vs_tangent + normalMapColor.g * vs_bitangent + normalMapColor.b * vs_normal);
	} else {
		fragNormal = vs_normal;
	}

	// TODO metallic, ... texture mapping

	out_GBuffer0 = vec4(fragPos, fragNormal.x);
	out_GBuffer1 = vec4(fragNormal.yz, diffuseColor.xy);
	out_GBuffer2 = vec4(diffuseColor.z, material.metallic, material.roughness, material.ao);
	out_GBuffer3 = vec4(material.emissive, 0, 0, 0);
	
}



