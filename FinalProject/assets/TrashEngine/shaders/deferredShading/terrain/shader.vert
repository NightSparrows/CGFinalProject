#version 450

layout (location = 0) in vec3 in_position;

// the position of the terrain
uniform ivec2 u_position;

layout (location = 0) out vec2 vs_texCoord;

const float c_terrainSize = 800.f;

void main() {
	
	vec2 actualPos = vec2(u_position) * c_terrainSize;
	
	gl_Position = vec4(in_position + vec3(actualPos.x, 0, actualPos.y), 1.0);
	
	vs_texCoord = in_position.xz / c_terrainSize;
	
}

