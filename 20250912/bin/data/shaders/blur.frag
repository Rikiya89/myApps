#version 150

uniform sampler2D uTex;
uniform vec2 uResolution; // texture resolution in pixels
uniform vec2 uDir;        // blur direction in pixels (e.g., (radius,0) or (0,radius))

in vec2 vTex;
out vec4 fragColor;

// 9-tap Gaussian kernel (rough)
void main(){
    vec2 texel = vec2(1.0) / uResolution;
    // uDir is in pixel units; convert to normalized tex coord
    vec2 dir = uDir * texel;

    vec4 sum = vec4(0.0);
    sum += texture(uTex, vTex - 4.0 * dir) * 0.05;
    sum += texture(uTex, vTex - 3.0 * dir) * 0.09;
    sum += texture(uTex, vTex - 2.0 * dir) * 0.12;
    sum += texture(uTex, vTex - 1.0 * dir) * 0.15;
    sum += texture(uTex, vTex)             * 0.18;
    sum += texture(uTex, vTex + 1.0 * dir) * 0.15;
    sum += texture(uTex, vTex + 2.0 * dir) * 0.12;
    sum += texture(uTex, vTex + 3.0 * dir) * 0.09;
    sum += texture(uTex, vTex + 4.0 * dir) * 0.05;

    fragColor = sum;
}

