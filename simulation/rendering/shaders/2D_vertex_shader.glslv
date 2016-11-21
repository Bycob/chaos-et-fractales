#version 330

uniform mat3 model;

uniform struct {
    float width;
    float height;
} screen;

in vec2 vert;
in vec2 texCoord;

out vec2 fragVert;
out vec2 fragTexCoord;

void main() {
    //Passage au fragment shader
    fragVert = vert;
    fragTexCoord = texCoord;

    //vec2 position = vec2(model * vec3(vert, 1));

    //Application des modifications de position.
    gl_Position = vec4(vert.x / screen.width * 2 - 1, vert.y / screen.height * 2 - 1, 999, 1);
}