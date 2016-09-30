#COMMON
#version 330 core

#VERTEX
uniform mat4 invProjection;
uniform mat4 invCamera;

layout (location = 0) in vec2 inPosition;
layout (location = 0) in vec2 inUV;

smooth out vec3 eyeDirection;

void main() {
    vec4 position = vec4(inPosition, -1, 1);
    gl_Position = position;
    eyeDirection = position.xyz * mat3(invCamera);
}

#FRAGMENT
uniform samplerCube skybox;

smooth in vec3 eyeDirection;
out vec4 fragmentColor;

void main() {
    fragmentColor = texture(skybox, eyeDirection);
}
