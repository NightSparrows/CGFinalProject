#version 450

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec3 in_tangent;
layout(location = 3) in vec2 in_texCoord;
layout (location = 4) in ivec4 in_bone;
layout (location = 5) in vec4 in_weight;

// pose matrices
const int MAX_WEIGHTS = 4;

layout (std140, binding = 2) buffer boneTransforms {
	mat4 boneMatrices[];
};

uniform mat4 u_transformationMatrix;
uniform mat4 u_projectionViewMatrix;
uniform ivec2 u_boneOffsets;
uniform float u_isStatic;

void main() {

	vec4 totalLocalPos;
	
	if (u_isStatic > 0.5) {
		totalLocalPos = vec4(in_position, 1.0);
	} else {
	
		mat4 boneTransform = mat4(0);
	
		for(int i = 0; i < MAX_WEIGHTS; i++) {
			boneTransform += boneMatrices[in_bone[i]] * in_weight[i];
		}
	
		
		totalLocalPos = boneTransform * vec4(in_position, 1.0);
	}
	gl_Position = u_projectionViewMatrix * u_transformationMatrix * totalLocalPos;


}


