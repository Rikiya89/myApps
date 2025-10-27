#version 150

in vec4 position;
in vec3 normal;
in vec4 color;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

out vec3 vNormal;
out vec3 vViewDir;
out vec4 vColor;

void main(){
    vec4 viewPos = modelViewMatrix * position;
    gl_Position = projectionMatrix * viewPos;

    vNormal  = normalize(normalMatrix * normal);
    vViewDir = normalize(-viewPos.xyz);
    vColor   = color;
}