#version 450

layout(location = 0) in vec2 in_texCoords[];

layout (std140, binding = 0) uniform globalBuffer {
	mat4 projectionMatrix;
	mat4 viewMatrix;
	vec4 cameraPosition;
	mat4 inverseProjectionMatrix;
}GlobalUbo;

layout (vertices = 4) out;
layout(location = 0) out vec2 out_texCoords[];


const int tessellationFactor = 5;
const float tessellationSlope = 0.5;
const float tessellationShift = 0.0;

float LodFactor(float dist) {
	
	float level = clamp(tessellationFactor / pow(dist, tessellationSlope) + tessellationShift, 0.0, 1.0);
	//float level = clamp(dist / 500.f, 0.0, 1.0);
	//level = 1.0 - level;
	
	
	float tes = mix(1, gl_MaxTessGenLevel, level);
		
	//const int MIN_TES = 1;
	//const int MAX_TES = 256;
	//const float MIN_DIST = 0.0;
	//const float MAX_DIST = 600.0;
	//int tes = int(mix(MAX_TES, MIN_TES, clamp(dist / MAX_DIST, 0.0, 1.0)));
	return tes;
}

void main() {
	
	if (gl_InvocationID == 0) {
	
        vec4 center0 = gl_in[0].gl_Position + (gl_in[3].gl_Position - gl_in[0].gl_Position) / 2.0; // left side
        vec4 center1 = gl_in[1].gl_Position + (gl_in[0].gl_Position - gl_in[1].gl_Position) / 2.0; // bot side
        vec4 center2 = gl_in[2].gl_Position + (gl_in[1].gl_Position - gl_in[2].gl_Position) / 2.0; // right side
        vec4 center3 = gl_in[3].gl_Position + (gl_in[2].gl_Position - gl_in[3].gl_Position) / 2.0; // top side


		gl_TessLevelOuter[0] = LodFactor(length(GlobalUbo.cameraPosition.xyz - center0.xyz));
		gl_TessLevelOuter[1] = LodFactor(length(GlobalUbo.cameraPosition.xyz - center1.xyz));
		gl_TessLevelOuter[2] = LodFactor(length(GlobalUbo.cameraPosition.xyz - center2.xyz));
		gl_TessLevelOuter[3] = LodFactor(length(GlobalUbo.cameraPosition.xyz - center3.xyz));

		gl_TessLevelInner[0] = (gl_TessLevelOuter[1] + gl_TessLevelOuter[3]) / 4;
		gl_TessLevelInner[1] = (gl_TessLevelOuter[0] + gl_TessLevelOuter[2]) / 4;

	}
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	out_texCoords[gl_InvocationID] = in_texCoords[gl_InvocationID];
}
