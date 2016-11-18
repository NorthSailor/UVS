#COMMON
#version 410 core

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

void main() {
    color = texture(tex, UV);
}
