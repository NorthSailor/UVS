#COMMON
#version 330 core
uniform mat4 transform;

#VERTEX
layout (location = 0) in vec2 aPosition;

void main() {
    gl_Position = transform * vec4(aPosition, 0, 1);
}

#FRAGMENT
out vec4 color;

void main() {
    color = vec4(1.0f, 1, 0.6f, 1);
}
