#version 460

layout (location = 0) in vec3 in_position;
layout (location = 1) in float in_rotation;
layout (location = 2) in float in_scale;
layout (location = 3) in vec4 in_texOffsets;
layout (location = 4) in float in_blend;

// binding global uniform buffer for
layout (binding = 0) uniform globalBuffer {
	mat4 projectionMatrix;
	mat4 viewMatrix;
	mat4 inverseProjectionMatrix;
	vec4 cameraPosition;
}GlobalUbo;

const vec2 quad[4] = vec2[](
	vec2(-1.0, -1.0),
	vec2( 1.0, -1.0),
	vec2(-1.0,  1.0),
	vec2( 1.0,  1.0)
);

layout (location = 0) out vec4 out_texOffsets;
layout (location = 1) out float out_blend;

uniform float u_numberOfRow;

void main() {

	mat4 transMatrix = mat4
		(
			vec4(GlobalUbo.viewMatrix[0][0], GlobalUbo.viewMatrix[1][0],GlobalUbo.viewMatrix[2][0],0),
			vec4(GlobalUbo.viewMatrix[0][1], GlobalUbo.viewMatrix[1][1],GlobalUbo.viewMatrix[2][1],0),
			vec4(GlobalUbo.viewMatrix[0][2], GlobalUbo.viewMatrix[1][2],GlobalUbo.viewMatrix[2][2],0),
			vec4(in_position, 1.0)
		);
	mat4 scaleMatrix = mat4
		(
			vec4(in_scale, 0, 0, 0),
			vec4(0, in_scale, 0, 0),
			vec4(0, 0, in_scale, 0),
			vec4(0, 0, 0, 1.0)
		);
	float rotSin = sin(in_rotation);
	float rotCos= cos(in_rotation);
	mat4 rotationMatrix = mat4
		(
			vec4(rotCos, -rotSin, 0, 0),
			vec4(rotSin,  rotCos, 0, 0),
			vec4(0, 0, 1.0, 0),
			vec4(0, 0, 0, 1.0)
		);
	mat4 transformationMatrix = transMatrix * scaleMatrix * rotationMatrix;
	vec4 worldPosition = transformationMatrix * vec4(quad[gl_VertexID], 0.0, 1.0);
	gl_Position = GlobalUbo.projectionMatrix * GlobalUbo.viewMatrix * worldPosition;

	vec2 texCoord = (quad[gl_VertexID] + vec2(1.0)) * 0.5;
	texCoord.y = 1.0 - texCoord.y;
	texCoord /= u_numberOfRow;
	out_texOffsets.xy = texCoord + in_texOffsets.xy;
	out_texOffsets.zw = texCoord + in_texOffsets.zw;

	out_blend = in_blend;
}