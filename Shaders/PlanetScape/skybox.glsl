#COMMON
#version 410 core

#VERTEX
uniform mat4 invTransform;

layout (location = 0) in vec2 inPosition;
layout (location = 0) in vec2 inUV;

smooth out vec3 eyeDirection;

void main() {
    vec4 position = vec4(inPosition, 0, 1);
    gl_Position = position;
    eyeDirection = vec3(invTransform * position);
}

#FRAGMENT
uniform samplerCube skybox;

smooth in vec3 eyeDirection;
out vec4 fragmentColor;

void main() {
    fragmentColor = texture(skybox, eyeDirection);
}
