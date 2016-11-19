#version 150

uniform mat4 projection;
uniform mat4 camera;

in vec3 vert;
in vec2 vertTexCoord;

out vec2 fragTexCoord;
out vec3 fragVert;

void main() {
    //Passage au fragment shader
    fragTexCoord = vertTexCoord;
    fragVert = vert;

    //Application des modifications de position.
    //TODO faire la transformation de la matrice directement dans le programme plutot qu'ici
    gl_Position = projection * mat4(mat3(camera)) * vec4(vert, 1);
}