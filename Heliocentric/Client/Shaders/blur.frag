#version 430 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D quadTexture;
uniform bool blurX;
uniform float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main(){
    vec2 texel_size = 1.0/ textureSize(quadTexture, 0);
    
    vec3 center = texture(quadTexture, TexCoords).rgb * weight[0];

    if(blurX){
        for(int i = 1; i < 5; i++){
            center += texture(quadTexture, TexCoords + vec2(texel_size.x * i, 0.0)).rgb * weight[i];
            center += texture(quadTexture, TexCoords - vec2(texel_size.x * i, 0.0)).rgb * weight[i];
        }
    
    }
    else{
        for(int i = 1; i < 5; i++){
            center += texture(quadTexture, TexCoords + vec2(0.0,texel_size.y * i)).rgb * weight[i];
            center += texture(quadTexture, TexCoords - vec2(0.0, texel_size.y * i)).rgb * weight[i];
        }
    
    }

    color = vec4(center, 1.0);
}
