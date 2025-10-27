#version 150

uniform sampler2DRect uTex0;
uniform vec2 uResolution;
in vec2 vTexCoord;
out vec4 fragColor;

void main(){
    vec2 texCoord = vTexCoord;
    float blurSize = 4.0;
    vec4 sum = vec4(0.0);

    // Simple 3x3 Gaussian blur kernel
    sum += texture(uTex0, texCoord + vec2(-blurSize, -blurSize)) * 0.0625;
    sum += texture(uTex0, texCoord + vec2(0.0, -blurSize)) * 0.125;
    sum += texture(uTex0, texCoord + vec2(blurSize, -blurSize)) * 0.0625;
    sum += texture(uTex0, texCoord + vec2(-blurSize, 0.0)) * 0.125;
    sum += texture(uTex0, texCoord) * 0.25;
    sum += texture(uTex0, texCoord + vec2(blurSize, 0.0)) * 0.125;
    sum += texture(uTex0, texCoord + vec2(-blurSize, blurSize)) * 0.0625;
    sum += texture(uTex0, texCoord + vec2(0.0, blurSize)) * 0.125;
    sum += texture(uTex0, texCoord + vec2(blurSize, blurSize)) * 0.0625;

    fragColor = sum;
}
