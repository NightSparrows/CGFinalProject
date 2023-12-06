#version 450

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec3 in_tangent;
layout(location = 3) in vec2 in_texCoord;

layout (location = 0) out vec3 vs_position;

layout (std140, binding = 0) uniform globalBuffer {
	mat4 projectionMatrix;
	mat4 viewMatrix;
	vec4 cameraPosition;
	mat4 inverseProjectionMatrix;
}GlobalUbo;

uniform mat4 u_transformationMatrix;

void main() {
	
	vec4 worldPosition = u_transformationMatrix * vec4(in_position, 1.0);
	
	mat4 viewMatrix = GlobalUbo.viewMatrix;
	viewMatrix[3][0] = 0;
	viewMatrix[3][1] = 0;
	viewMatrix[3][2] = 0;
	
	gl_Position = GlobalUbo.projectionMatrix * viewMatrix * u_transformationMatrix * vec4(in_position, 1.0);
	
	vs_position = worldPosition.xyz;
	
}
