#version 330

uniform mat4 projection;
uniform mat4 camera;
uniform mat4 model;

uniform vec3 cameraPos;

uniform vec3 color;

uniform int count;

in vec3 fragVert;
in float cameraFactor;
in float id;

out vec4 finalColor;

void main() {
    finalColor = vec4(color, 0.3) //Couleur de base
            + vec4(max((id + 50 - count) / 40.0, 0)) //en fonction de l'ID
            + vec4(vec3(cameraFactor * 0.5), cameraFactor * 0.7); //en fonction de la camera
}