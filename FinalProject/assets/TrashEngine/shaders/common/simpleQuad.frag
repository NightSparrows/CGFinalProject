#version 450

uniform sampler2D textureSampler;

layout (location = 0) in vec2 texCoord;

layout (location = 0) out vec4 out_Color;

void main() {

	out_Color = texture(textureSampler, texCoord);

}

