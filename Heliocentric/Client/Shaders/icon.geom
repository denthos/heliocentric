#version 430 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 projection;

in VS_OUT {
    vec4 color;
    //float size;
} gs_in[];

out vec4 fColor;

void build_icon(vec4 position)
{    
    fColor = gs_in[0].color; // gs_in[0] since there's only one input vertex
    gl_Position = position + vec4(-0.01, -0.0175, 0.0, 0.0); // 1:bottom-left   
    EmitVertex();   
    gl_Position = position + vec4( 0.01, -0.0175, 0.0, 0.0); // 2:bottom-right
    EmitVertex();
    gl_Position = position + vec4(-0.01,  0.0175, 0.0, 0.0); // 3:top-left
    EmitVertex();
    gl_Position = position + vec4( 0.01,  0.0175, 0.0, 0.0); // 4:top-right
    EmitVertex();
    EndPrimitive();
}

void main() {    
    build_icon(gl_in[0].gl_Position);
}
