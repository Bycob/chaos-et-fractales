#version 330

uniform mat4 camera;

uniform vec3 color;

in vec3 fragVert;
in float id;

out vec4 finalColor;

void main() {
    finalColor = vec4(1, 1, 1, 1);
}