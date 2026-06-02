#version 330 core

in vec4 vColor;
in vec2 vTexCoord;

uniform sampler2D outTexture;
uniform bool isTextured; 

out vec4 FragColor;

void main(){
    if(isTextured)
        FragColor = texture(outTexture,vTexCoord) * vColor;
    else
        FragColor = vColor;
}