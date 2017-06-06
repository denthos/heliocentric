#version 430 core

layout (std140) uniform material {
    vec3 m_diffuse;
    vec3 m_ambient;
    vec3 m_specular;
    vec3 m_emissive;
    float m_shininess;
}; 

in vec3 Normal;
in vec3 FragPos;
uniform float time;
uniform bool explode_on;

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 buffer1_color;

uniform vec3 viewPos;

vec3 calcSunLight(vec3 normal, vec3 fragPos, vec3 viewDir) {
     vec3 lerpColor = m_diffuse;
     if(explode_on){
     float stop1 = 0.7;
 
     vec3 red = vec3(1.0,0.0,0.0);
     vec3 black = vec3(0.0);
     vec3 toColor;
 
     float factor = (sin(time) + 1.0f) / 2.0f;
 
     if(factor <= stop1){toColor = red;}
     else{
         lerpColor = red;
         toColor = black;
     }
 
     lerpColor = (1-factor) * lerpColor + (factor) * toColor;
     }
     vec3 lightDir = normalize(-fragPos);
     // Diffuse shading
     float diff = max(dot(normal, lightDir), 0.0f);
     // Specular shading
     vec3 reflectDir = reflect(-lightDir, normal);
     float spec = pow(max(dot(viewDir, reflectDir), 0.0f), m_shininess);
     // Combine results
     vec3 ambient = vec3(0.0f);
     vec3 diffuse = diff * lerpColor;
     vec3 specular = spec * vec3(0.5f);// * m_specular;
 	specular = clamp(specular, 0.0f, 1.0f);
     return (ambient + diffuse + specular)* lerpColor;
 }


void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = calcSunLight( norm, FragPos, viewDir);
	color = vec4(m_diffuse, 1.0f) * vec4(result, 1.0f);

  buffer1_color = vec4(vec3(0.0), 1.0);

}
