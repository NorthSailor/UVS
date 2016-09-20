#COMMON
#version 330 core
uniform mat4 transform;

#VERTEX
layout (location = 0) in vec2 aPosition;

void main() {
    vec3 position = vec3(aPosition.x, aPosition.y, sin(0.25f * aPosition.x + aPosition.y) * 0.1f);
    gl_Position = transform * vec4(position, 1);
}

#FRAGMENT
out vec4 color;

void main() {
    color = vec4(1.0f, 1, 0.6f, 1);
}
