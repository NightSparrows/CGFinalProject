#version 460 core

layout (location = 0) in vec4 in_position;
layout (location = 1) in vec2 in_uv;
layout (location = 2) in vec3 in_normal;

layout(location = 0) out vec4 out_GBuffer0;
layout(location = 1) out vec4 out_GBuffer1;
layout(location = 2) out vec4 out_GBuffer2;
layout(location = 3) out vec4 out_GBuffer3;
layout(location = 4) out vec4 out_GBuffer4;

void main() {

	// normal
	vec3 fragNormal = in_normal;
	
	vec3 materialAmbient = vec3(0);
	vec3 materialDiffuse = vec3(1);
	vec3 materialSpecular = vec3(0);
	float shininess = 0;
	float reflectivity = 0;
	vec3 diffuseColor = vec3(1, 1, 1);		// for testing
	
	out_GBuffer0 = vec4(in_position.xyz, fragNormal.x);
	out_GBuffer1 = vec4(fragNormal.yz, materialAmbient.xy);
	out_GBuffer2 = vec4(materialAmbient.z, materialDiffuse);
	out_GBuffer3 = vec4(materialSpecular, shininess);
	out_GBuffer4 = vec4(reflectivity, diffuseColor);
	
}



