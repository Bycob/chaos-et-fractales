#version 150

uniform mat4 projection;
uniform mat4 camera;
uniform mat4 model;

in vec3 vert;
in vec3 vertNorm;

out vec3 fragVert;
out vec3 fragNorm;

void main() {
    //Passage au fragment shader
    fragVert = vert;
    fragNorm = vertNorm;

    //Application des modifications de position.
    gl_Position = projection * camera * model * vec4(vert, 1);
}