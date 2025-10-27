#version 150

uniform float uPointSize;
uniform float uTime;

in vec4  position;
in vec4  color;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

out vec4 vColor;

void main(){
    vec4 viewPos = modelViewMatrix * position;
    // Distance-aware size (keeps points readable)
    float dist = length(viewPos.xyz);
    float atten = clamp(350.0 / dist, 0.6, 2.5); // tweakable
    gl_PointSize = uPointSize * atten;

    gl_Position = projectionMatrix * viewPos;
    vColor = color;
}