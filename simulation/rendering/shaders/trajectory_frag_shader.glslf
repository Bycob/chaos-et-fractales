#version 330

uniform mat4 camera;

uniform vec3 color;

in vec3 fragVert;
in float id;

out vec4 finalColor;

void main() {
    finalColor = 0.5 * vec4(1 - id / 10000, 1 - id / 10000, id / 10000, 2);
}