#version 150

uniform sampler2D uTex;
uniform vec2 uResolution; // not used, kept for flexibility
uniform float uThreshold; // 0..1

in vec2 vTex;
out vec4 fragColor;

void main(){
    vec3 c = texture(uTex, vTex).rgb; // GL_TEXTURE_2D with normalized texcoords
    float lum = dot(c, vec3(0.2126, 0.7152, 0.0722));
    float m = smoothstep(uThreshold, 1.0, lum);
    fragColor = vec4(c * m, m);
}

