#version 450 core

layout(location = 0) in vec3 normal;
layout(location = 1) in vec3 eye;
layout(location = 2) in vec3 direction;

layout(location = 3) uniform vec4 diffuse;
layout(location = 4) uniform vec4 ambient;
layout(location = 5) uniform vec4 specular;
layout(location = 6) uniform vec4 emissive;
layout(location = 7) uniform float shininess;
layout(location = 8) uniform int texCount;

layout(location = 0) vec4 color;

void main() {
	float intensity = max(dot(normal,direction), 0.0);

	vec4 spec = vec4(0.0);
	if (intensity > 0.0) {
		vec3 h = normalize(direction + eye);
		float intSpec = max(dot(h,normal), 0.0);
		spec = specular * pow(intSpec, shininess);
	}
	
	color = max(intensity * diffuse + spec, ambient);
}