#version 450 core

in vec3 light_normal;
in vec3 light_eye;
in vec3 light_direction;

layout (std140) uniform fragment {
    vec4 diffuse;
    vec4 ambient;
    vec4 specular;
    vec4 emissive;
    float shininess;
    int texCount;
};

layout(location = 0) vec4 color;

void main() {
	float intensity = max(dot(light_normal, light_direction), 0.0);

	vec4 spec = vec4(0.0);
	if (intensity > 0.0) {
		vec3 h = normalize(light_direction + light_eye);
		float intSpec = max(dot(h,light_normal), 0.0);
		spec = specular * pow(intSpec, shininess);
	}
	
	color = max(intensity * diffuse + spec, ambient);
}