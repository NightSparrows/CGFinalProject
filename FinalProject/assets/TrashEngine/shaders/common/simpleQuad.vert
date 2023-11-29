#version 450

const vec2 quad[4] = vec2[](
	vec2(-1.0, -1.0),
	vec2( 1.0, -1.0),
	vec2(-1.0,  1.0),
	vec2( 1.0,  1.0)
);

layout (location = 0) out vec2 texCoord;

void main() {

	gl_Position = vec4(quad[gl_VertexID], 0.0, 1.0);

	texCoord = (quad[gl_VertexID] + vec2(1.0)) * 0.5;

}


