#version 430 core

layout (std140) uniform material {
    vec3 m_diffuse;
    vec3 m_ambient;
    vec3 m_specular;
    vec3 m_emissive;
    float m_shininess;
}; 

struct PointLight {
	vec3 position;

	float quadratic;
	float linear;
	float constant;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;


layout (location = 0) out vec4 color;
layout (location = 1) out vec4 buffer1_color;

uniform vec3 viewPos;
uniform vec3 m_color;
uniform PointLight pointLight;
uniform bool glow;


vec3 calcSunLight(vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(-fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0f);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), m_shininess);
    // Combine results
    vec3 ambient = m_ambient;
    vec3 diffuse = diff * m_diffuse;
    vec3 specular = spec * m_specular;
    specular = clamp(specular, 0.0f, 1.0f);
    return (ambient + diffuse + specular);
}


void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = calcSunLight( norm, FragPos, viewDir);
	color = vec4(m_color * m_diffuse, 1.0f) * vec4(result, 1.0f);

	if(glow){
		buffer1_color = color;
	}
	else{
		buffer1_color = vec4(vec3(0.0), 1.0);
	}


}
