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
    vec2 offsets[9] = vec2[](
        vec2(-uStep, vStep), // top-left
        vec2(0.0f, vStep), // top-center
        vec2(uStep, vStep), // top-right
        vec2(-uStep, 0.0f), // center-left
        vec2(0.0f, 0.0f), // center-center
        vec2(uStep, 0.0f), // center-right
        vec2(-uStep, -vStep), // bottom-left
        vec2(0.0f, -vStep), // bottom-center
        vec2(uStep, -vStep) // bottom-right
    );

	float kernel[9] = float[](
		1, 2, 1,
		2, -11, 2,
		1, 2, 1
	);

	for (int i = 0; i < 9; i++) {
		color += kernel[i] * texture2D(tex, UV + offsets[i]);
	}

	if (gl_FragCoord.x < 590) {
		color.g += color.r + color.b;
		color.g *= 0.33f;
		color.g = 1 - color.g;
		color.r = color.g;
		color.b = color.g;
	} else {
		color = texture2D(tex, UV);
	}

	color.a = 1.0f;
}
