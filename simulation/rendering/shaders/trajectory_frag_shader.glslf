#version 330

uniform mat4 projection;
uniform mat4 camera;
uniform mat4 model;

uniform vec3 cameraPos;

uniform vec3 color;
uniform float alpha;
uniform int count;

in vec3 fragVert;
in float cameraFactor;
in float id;

out vec4 finalColor;

void main() {
    vec3 idColor = vec3(max((id + 50 - count) / 40.0, 0)); //en fonction de l'ID
    vec3 cameraColor = vec3(cameraFactor * 0.5); //en fonction de la camera
    float multiplier = (0.2 + cameraFactor * 0.8);

    vec3 totalColor = (color + idColor + cameraColor) * min(1, multiplier);

    finalColor = vec4(totalColor, alpha);
}