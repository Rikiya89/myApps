#version 150

in vec4 vColor;
out vec4 fragColor;

void main(){
    // gl_PointCoord in [0,1]; make a soft round disc
    vec2 uv = gl_PointCoord * 2.0 - 1.0;
    float r = length(uv);

    // Smooth edge + brighter core
    float alpha = smoothstep(1.0, 0.55, 1.0 - r);
    float core  = smoothstep(0.25, 0.0, r);

    vec3 col = vColor.rgb * (0.65 + 0.35 * core);
    fragColor = vec4(col, vColor.a * alpha);
}