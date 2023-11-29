#version 450

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec3 in_tangent;
layout(location = 3) in vec2 in_texCoord;

layout (std140, binding = 0) uniform globalBuffer {
	mat4 projectionMatrix;
	mat4 viewMatrix;
	vec4 cameraPosition;
}GlobalUbo;

uniform mat4 u_transformationMatrix;
uniform mat4 u_tiModelMatrix;

// output
out vec3 fragPos;
out vec2 texCoord;
layout (location = 2) out vec3 vs_normal;		// normal in world space
layout (location = 3) out vec3 vs_tangent;		// tangent in world space
layout (location = 4) out vec3 vs_bitangent;	// bitangent in world space

void main() {
	
	vec4 worldPosition = u_transformationMatrix * vec4(in_position, 1.0);
	gl_Position = GlobalUbo.projectionMatrix * GlobalUbo.viewMatrix * worldPosition;
	//gl_Position = worldPosition;
	
	fragPos = worldPosition.xyz;
	texCoord = in_texCoord;

	vs_normal = normalize((u_tiModelMatrix * vec4(in_normal, 0.0)).xyz);
	vs_tangent = normalize((u_tiModelMatrix * vec4(in_tangent, 0.0)).xyz);
	vs_bitangent = cross(vs_normal, vs_tangent);
	
}

