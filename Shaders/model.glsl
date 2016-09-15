#version 330 core
#VERTEX
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 UV;

uniform mat4 ViewProjection;
uniform mat4 ModelTransform;

out VS_OUT {
    vec3 normal;
    vec2 UV;
} vs_out;

void main() {
    gl_Position = ViewProjection * ModelTransform * vec4(position, 1);
    vs_out.normal = normal;
    vs_out.UV = UV;
}

#FRAGMENT
uniform sampler2D diffuseSampler;

in VS_OUT {
    vec3 normal;
    vec2 UV;
} fs_in;

out vec4 color;
vec3 lightDir = vec3(-6, -20, -0.5);

void main() {
    vec4 diffuse = texture2D(diffuseSampler, fs_in.UV);
    vec3 nLightDir = normalize(lightDir);
    float intensity = 2 * clamp(-(dot(fs_in.normal, nLightDir)), 0.0f, 1.0f);
    vec4 ambient = 0.2 * vec4(1, 0.4, 0.4, 1) * diffuse;
    color = intensity * diffuse;
    color.a = clamp(diffuse.a + 0.2f, 0.0f, 1.0f);
}
