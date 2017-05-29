#version 430 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 projection;

in VS_OUT {
    vec4 color;
    float size;
} gs_in[];

out vec4 fColor;

void build_quad(vec4 position)
{    
    fColor = gs_in[0].color; // gs_in[0] since there's only one input vertex
    float size = gs_in[0].size;

    vec4 bottom_left = position + vec4(-4.5f, -2.5f, 0.0f, 0.0f) * size;    // 1:bottom-left   
    gl_Position = projection * bottom_left;
    EmitVertex();   

    vec4 bottom_right = position + vec4( 4.5f, -2.5f, 0.0f, 0.0f) * size;    // 2:bottom-right
    gl_Position = projection * bottom_right;
    EmitVertex();

    vec4 top_left = position + vec4(-4.5f,  2.5f, 1.0f, 0.0f) * size;    // 3:top-left
    gl_Position = projection * top_left;
    EmitVertex();

    vec4 top_right = position + vec4( 4.5f,  2.5f, 0.0f, 0.0f) * size;    // 4:top-right
    gl_Position = projection * top_right;
    EmitVertex();

    EndPrimitive();
}

void main() {    
    build_quad(gl_in[0].gl_Position);
}
