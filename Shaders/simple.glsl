#version 330 core
uniform sampler2D skySampler;
uniform sampler2D earthSampler;
uniform float mixingFactor;

#VERTEX
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 UV;
layout (location = 2) in vec4 color;

out VS_OUT {
	vec2 UV;
	vec2 plainUV;
	vec4 color;
} vs_out;

void main() {
	gl_Position = vec4(2 * pos, 1);
	// We need to flip the V coordinate for most models.
	vs_out.UV = vec2(UV.x, 1 - UV.y);
	vs_out.plainUV = vs_out.UV;
	vs_out.UV.y = mix(vs_out.UV.y, 0.5, 1 - mixingFactor);
	vs_out.UV.x = mix(vs_out.UV.x, 0.5, 1 - mixingFactor);
	vs_out.color = color;
}

#FRAGMENT
in VS_OUT {
	vec2 UV;
	vec2 plainUV;
	vec4 color;
} fs_in;

out vec4 fragColor;

void main() {
	vec4 skyDiffuse = texture(skySampler, fs_in.plainUV);
	vec4 earthDiffuse = texture(earthSampler, fs_in.UV);
	fragColor = mix(skyDiffuse, earthDiffuse, pow(mixingFactor, 0.5));
}
