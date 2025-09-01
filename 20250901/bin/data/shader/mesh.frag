#version 150

uniform float uTime;
uniform float uRimStrength;     // 0..1
uniform vec3  uRimColor;

in vec3 vNormal;
in vec3 vViewDir;
in vec4 vColor;

out vec4 fragColor;

void main(){
    // Base color from vertex
    vec3 base = vColor.rgb;

    // Fresnel rim (edge glow)
    float ndv = clamp(dot(normalize(vNormal), normalize(vViewDir)), 0.0, 1.0);
    float fres = pow(1.0 - ndv, 2.0);  // smoother, broader rim

    // Subtle time shimmer
    float shimmer = 0.35 + 0.65 * 0.5 * (sin(uTime * 1.7 + ndv * 6.2831) + 1.0);

    vec3 rim = uRimColor * (uRimStrength * fres * shimmer);

    // Combine with gentle filmic-ish curve
    vec3 col = base + rim;
    col = col / (col + vec3(1.0));   // simple tone-map

    fragColor = vec4(col, 1.0);
}