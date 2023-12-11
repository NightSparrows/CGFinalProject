#version 450

layout (location = 0) in vec4 in_texOffsets;
layout (location = 1) in float in_blend;

layout (location = 0) out vec4 out_Color;

layout (location = 0) uniform sampler2D u_particleTexture;

void main() {
	


	//out_Color = vec4(1.0);
	vec4 texColor1 = texture(u_particleTexture, in_texOffsets.xy);
	vec4 texColor2 = texture(u_particleTexture, in_texOffsets.zw);

	out_Color = mix(texColor1, texColor2, in_blend);

}
