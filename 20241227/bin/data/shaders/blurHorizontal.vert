#version 150

uniform sampler2DRect tex0;
uniform float blurAmount;

in vec2 texCoordVarying;
out vec4 fragColor;

void main() {
    vec4 sum = vec4(0.0);
    // Simple "box blur" across 9 samples
    for (int i = -4; i <= 4; i++){
        float offset = float(i) * blurAmount;
        sum += texture(tex0, texCoordVarying + vec2(offset, 0.0));
    }
    fragColor = sum / 9.0;
}
