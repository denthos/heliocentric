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
in vec2 TexCoord;

//color attachment 1
layout(location = 0) out vec4 sceneColor;
layout(location = 1) out vec4 brightColor;

uniform vec3 viewPos;
uniform PointLight pointLight;
uniform sampler2D ourTexture;
uniform float threshold;

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
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

	vec3 result = calcPointLight(pointLight, norm, FragPos, viewDir);
	vec4 color = texture(ourTexture, TexCoord) * vec4(result, 1.0f);

	sceneColor = color;

	float luminance = color.r * 0.3 + color.g * 0.59 + color.b * 0.11;

	if(luminance > 0.03){ brightColor = color;}
	else{ brightColor = vec4(vec3(0.0), 1.0);}


}
