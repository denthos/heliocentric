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
uniform PointLight pointLight;


vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), m_shininess);
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // Combine results
    vec3 ambient = light.ambient  * m_diffuse; //multiply by material ambient
    vec3 diffuse = light.diffuse * diff;
    vec3 specular = light.specular * spec;// * m_specular;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return ( ambient + diffuse + specular) * m_diffuse;
}

vec3 calcSunLight(vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(-fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0f);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), m_shininess);
    // Combine results
    vec3 ambient = vec3(0.0f);
    vec3 diffuse = diff * m_diffuse;
    vec3 specular = spec * vec3(0.5f);// * m_specular;
	specular = clamp(specular, 0.0f, 1.0f);
    return (ambient + diffuse + specular)* m_diffuse;
}

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = calcSunLight(norm, FragPos, viewDir);
	color = vec4(result, 1.0f);


}
