#version 330

uniform sampler2D tex;

in vec2 fragVert;
in vec2 fragTexCoord;

out vec4 finalColor;

void main() {
    finalColor = texture(tex, fragTexCoord);
}