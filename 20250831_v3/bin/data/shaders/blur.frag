#version 150
in vec2 vTexCoord;
out vec4 fragColor;

uniform sampler2D uTex0;
uniform vec2 uTexel;  // (1/width, 1/height)
uniform vec2 uDir;    // (1,0)=horizontal, (0,1)=vertical

// 9-tap Gaussian kernel (normalized)
const float w0 = 0.227027;
const float w1 = 0.1945946;
const float w2 = 0.1216216;
const float w3 = 0.054054;
const float w4 = 0.016216;

void main(){
    vec2 uv = vTexCoord;
    vec4 c = texture(uTex0, uv) * w0;

    float spread = 1.5;

    vec2 off1 = uDir * uTexel * 1.0 * spread;
    vec2 off2 = uDir * uTexel * 2.0 * spread;
    vec2 off3 = uDir * uTexel * 3.0 * spread;
    vec2 off4 = uDir * uTexel * 4.0 * spread;

    c += texture(uTex0, uv + off1) * w1 + texture(uTex0, uv - off1) * w1;
    c += texture(uTex0, uv + off2) * w2 + texture(uTex0, uv - off2) * w2;
    c += texture(uTex0, uv + off3) * w3 + texture(uTex0, uv - off3) * w3;
    c += texture(uTex0, uv + off4) * w4 + texture(uTex0, uv - off4) * w4;

    fragColor = c;
}