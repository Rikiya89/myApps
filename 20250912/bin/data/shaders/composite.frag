#version 150

uniform sampler2D uTex;
in vec2 vTex;
out vec4 fragColor;

void main(){
    fragColor = texture(uTex, vTex);
}

