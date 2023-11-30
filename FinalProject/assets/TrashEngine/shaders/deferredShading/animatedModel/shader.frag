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
	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
	float shininess;
	float reflectivity;
	float hasDiffuseTexture;
	vec3 diffuseColor;
	float hasNormalTexture;
};

// use storage buffer?
// a temp buffer struct for what data in material
layout (std430, binding = 1) buffer material {
/*
	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
	float shininess;
	float reflectivity;
	float hasDiffuseTexture;
	vec3 diffuseColor;
	float hasNormalTexture;
	*/
	vec4 data[4];
}materialData;

layout(location = 0) uniform sampler2D diffuseSampler;
layout(location = 1) uniform sampler2D normalTextureSampler;

void main() {

/*
	vec3 materialDiffuse = materialData.diffuse;
	vec3 materialAmbient = materialData.ambient;
	vec3 materialSpecular = materialData.specular;
	float shininess = materialData.shininess;
	float reflectivity = materialData.reflectivity;
	float hasDiffuseTexture = materialData.hasDiffuseTexture;
	vec3 materialDiffuseColor = materialData.diffuseColor;
	float hasNormalTexture = materialData.hasNormalTexture;
	*/

	vec3 materialDiffuse = materialData.data[0].xyz;
	vec3 materialAmbient = vec3(materialData.data[0].w, materialData.data[1].xy);
	vec3 materialSpecular = vec3(materialData.data[1].zw, materialData.data[2].x);
	float shininess = materialData.data[2].y;
	float reflectivity = materialData.data[2].z;
	float hasDiffuseTexture = materialData.data[2].w;
	vec3 materialDiffuseColor = materialData.data[3].xyz;
	float hasNormalTexture = materialData.data[3].w;

	vec3 diffuseColor;

	if (hasDiffuseTexture > 0) {
		diffuseColor = texture(diffuseSampler, texCoord).rgb;
	} else {
		diffuseColor = materialDiffuseColor;
	}
	//diffuseColor = materialDiffuseColor;
	
	vec3 fragNormal;
	if (hasNormalTexture > 0) {
		vec3 normalMapColor = texture(normalTextureSampler, texCoord).rgb * 2.0 - 1.0;
		fragNormal = vec3(normalMapColor.r * vs_tangent + normalMapColor.g * vs_bitangent + normalMapColor.b * vs_normal);
	} else {
		fragNormal = vs_normal;
	}
	
	out_GBuffer0 = vec4(fragPos, fragNormal.x);
	out_GBuffer1 = vec4(fragNormal.yz, materialAmbient.xy);
	out_GBuffer2 = vec4(materialAmbient.z, materialDiffuse);
	out_GBuffer3 = vec4(materialSpecular, shininess);
	out_GBuffer4 = vec4(reflectivity, diffuseColor);
	
}



