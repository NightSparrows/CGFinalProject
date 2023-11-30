#version 450

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec3 in_tangent;
layout(location = 3) in vec2 in_texCoord;
layout(location = 4) in ivec4 in_boneID;
layout(location = 5) in vec4 in_weight;

layout (std140, binding = 0) uniform globalBuffer {
	mat4 projectionMatrix;
	mat4 viewMatrix;
	vec4 cameraPosition;
}GlobalUbo;

const int MAX_WEIGHTS = 4;

layout (std140, binding = 2) buffer boneTransforms {
	mat4 boneMatrices[];
};

uniform mat4 u_transformationMatrix;
uniform mat4 u_tiModelMatrix;

// output
out vec3 fragPos;
out vec2 texCoord;
layout (location = 2) out vec3 vs_normal;		// normal in world space
layout (location = 3) out vec3 vs_tangent;		// tangent in world space
layout (location = 4) out vec3 vs_bitangent;	// bitangent in world space

void main() {
	
	mat4 boneTransform = mat4(0);
	
	for(int i = 0; i < MAX_WEIGHTS; i++) {
		boneTransform += boneMatrices[in_boneID[i]] * in_weight[i];
	}
	
	vec4 worldPosition = u_transformationMatrix * boneTransform * vec4(in_position, 1.0);
	gl_Position = GlobalUbo.projectionMatrix * GlobalUbo.viewMatrix * worldPosition;
	//gl_Position = worldPosition;
	
	fragPos = worldPosition.xyz;
	texCoord = in_texCoord;

	vs_normal = normalize((u_tiModelMatrix * boneTransform * vec4(in_normal, 0.0)).xyz);
	vs_tangent = normalize((u_tiModelMatrix * boneTransform * vec4(in_tangent, 0.0)).xyz);
	vs_bitangent = cross(vs_normal, vs_tangent);
	
}

