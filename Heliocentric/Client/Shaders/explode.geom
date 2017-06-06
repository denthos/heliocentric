#version 430 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VERT_OUT{
	vec3 Normal;
	vec3 FragPos;
} geo_in[];

out vec3 Normal;
out vec3 FragPos;

uniform float time;
uniform bool explode_on;

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 52.0f;
	if(!explode_on){
		return position;
    }
    vec3 direction = normal * ((sin(time) + 1.0f) / 2.0f) * magnitude; 
    return position + vec4(direction, 0.0f);
}

vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

void main() {    
    vec3 normal = GetNormal(); //get the face normal

    gl_Position = explode(gl_in[0].gl_Position, normal);
    Normal = geo_in[0].Normal;
    FragPos = geo_in[0].FragPos;
    EmitVertex();

    gl_Position = explode(gl_in[1].gl_Position, normal);
    Normal = geo_in[1].Normal;
    FragPos = geo_in[1].FragPos;
    EmitVertex();

    gl_Position = explode(gl_in[2].gl_Position, normal);
    Normal = geo_in[2].Normal;
    FragPos = geo_in[2].FragPos;
    EmitVertex();
    EndPrimitive();
}
