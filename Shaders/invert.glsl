#COMMON
#version 330 core

#VERTEX
layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 inUV;

out vec2 UV;

void main() {
    gl_Position = vec4(inPos, 0, 1);
    UV = inUV;
}

#FRAGMENT
in vec2 UV;
out vec4 color;

uniform sampler2D tex;
uniform float uStep;
uniform float vStep;

void main() {
    float offset = uStep + vStep;
    offset *= 0.5f;
    vec2 offsets[9] = vec2[](
    vec2(-offset, offset), // top-left
    vec2(0.0f,
    offset), // top-center
    vec2(offset, offset), // top-right
    vec2(-offset, 0.0f),
    // center-left
    vec2(0.0f,
    0.0f),
    // center-center
    vec2(offset, 0.0f),
    // center-right
    vec2(-offset, -offset), // bottom-left
    vec2(0.0f,
    -offset), // bottom-center
    vec2(offset, -offset) // bottom-right
    );

	float kernel[9] = float[](
		-1, -1, -1,
		-1, 9, -1,
		-1, -1, -1
	);

	for (int i = 0; i < 9; i++) {
		color += kernel[i] * texture2D(tex, UV + offsets[i]);
	}
	color.g = color.r + color.g + color.b;
	color.g *= 0.33f;
	color.r = 0;
	color.b = 0;
	color.a = 1.0f;
}
