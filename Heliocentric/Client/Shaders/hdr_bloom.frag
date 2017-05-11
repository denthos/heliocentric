#version 430 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D sceneTexture;
uniform sampler2D blurTexture;
uniform float gammaFactor;
uniform float exposure;

void main(){
    float gamma = 0.75;
    if(gamma <= 0.0){gamma = 1.0f;} //make sure factor is positive

    vec3 sceneColor = texture(sceneTexture, TexCoords).rgb;
    vec3 blurColor = texture(blurTexture, TexCoords).rgb;
    vec3 comboColor = sceneColor + blurColor;
    vec3 hdrColor = vec3(1.0) -exp( -comboColor* exposure);
    vec3 gammaCorrection = pow(hdrColor , vec3(1.0/gamma));
    color = vec4(gammaCorrection, 1.0f);


}
