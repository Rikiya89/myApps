#version 150

uniform sampler2DRect tex0;
uniform float blurAmount;

in vec2 texCoordVarying;
out vec4 fragColor;

void main() {
    vec4 sum = vec4(0.0);
    for (int j = -4; j <= 4; j++){
        float offset = float(j) * blurAmount;
        sum += texture(tex0, texCoordVarying + vec2(0.0, offset));
    }
    fragColor = sum / 9.0;
}
