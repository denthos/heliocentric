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

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform vec3 viewPos;

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
    return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0f);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), m_shininess);
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.quadratic * (distance * distance));
    // Combine results
    vec3 ambient = light.ambient * m_ambient;
    vec3 diffuse = light.diffuse * diff * m_diffuse;
    vec3 specular = light.specular * spec * m_specular;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular );
}

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = calcSunLight(norm, FragPos, viewDir);
	color = vec4(result, 1.0f);
}
