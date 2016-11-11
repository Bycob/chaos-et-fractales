#version 330

uniform mat4 camera;

uniform vec3 color;

in vec3 fragVert;
in float id;

out vec4 finalColor;

void main() {
    finalColor = 1.0 * vec4(color, 2);
}