#version 330

uniform mat4 projection;
uniform mat4 camera;
uniform mat4 model;

uniform vec3 cameraPos;

uniform vec3 color;

uniform int count;

in vec3 fragVert;
in float id;

out vec4 finalColor;

void main() {

    finalColor = vec4(max((id + 50 - count) / 40.0, 0)) + vec4(color, 0.4);
}