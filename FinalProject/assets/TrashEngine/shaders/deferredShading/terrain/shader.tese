#version 450

layout (quads, equal_spacing, cw) in;

layout (location = 0) in vec2 in_texCoords[];

layout (std140, binding = 0) uniform globalBuffer {
	mat4 projectionMatrix;
	mat4 viewMatrix;
	vec4 cameraPosition;
	mat4 inverseProjectionMatrix;
}GlobalUbo;

layout (binding = 0) uniform sampler2D u_heightMap;
layout (binding = 1) uniform sampler2D u_normalMap;

uniform float u_heightIntensity;

layout (location = 0) out vec4 out_position;
layout (location = 1) out vec2 out_uv;
layout (location = 2) out vec3 out_normal;

void main() {
	
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;
	
	vec2 uv0 = in_texCoords[0];
	vec2 uv1 = in_texCoords[1];
	vec2 uv2 = in_texCoords[2];
	vec2 uv3 = in_texCoords[3];
	
	vec2 leftUV = mix(uv0, uv3, v);
	vec2 rightUV = mix(uv1, uv2, v);
	vec2 texCoord = leftUV + u * (rightUV - leftUV);
	
	vec4 pos0 = gl_in[0].gl_Position;
	vec4 pos1 = gl_in[1].gl_Position;
	vec4 pos2 = gl_in[2].gl_Position;
	vec4 pos3 = gl_in[3].gl_Position;
	
	vec4 leftPos = mix(pos0, pos3, v);
	vec4 rightPos = mix(pos1, pos2, v);
	vec4 pos = leftPos + u * (rightPos - leftPos);
	
	float height = texture(u_heightMap, texCoord).r;
	height -= 0.5;
	height /= 0.5;
	height *= u_heightIntensity;
	pos.y = height/* - [terrain height offset] */;
	
	gl_Position = GlobalUbo.projectionMatrix * GlobalUbo.viewMatrix * pos;
	out_uv = texCoord;

	out_position = pos;
	
	//vec3 normal = (2.0 * texture(u_normalMap, texCoord) - 1).rgb;
	vec3 normal = (2.0 * texture(u_normalMap, texCoord) - 1).gbr;		// because normal is point upward
	out_normal = normalize(normal);

	
}
