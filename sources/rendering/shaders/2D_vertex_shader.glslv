#version 330

uniform mat3 model;

uniform struct {
    int width;
    int height;
} screen;

in vec2 vert;
in vec2 texCoord;

out vec2 fragVert;
out vec2 fragTexCoord;

void main() {
    //Passage au fragment shader
    fragVert = vert;
    fragTexCoord = texCoord;

    vec2 position = vec2(model * vec3(vert, 1));

    //Application des modifications de position.
    float w = screen.width;
    float h = screen.height;
    gl_Position = vec4(position.x / w * 2.0 - 1, - position.y / h * 2.0 + 1, -1, 1);
}