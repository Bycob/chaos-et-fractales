#version 150

uniform sampler2D tex;

in vec3 fragVert;
in vec2 fragTexCoord;

out vec4 finalColor;

void main() {
    //On a simplement la couleur de la cubemap.
    finalColor = texture(tex, fragTexCoord) - vec4(0.1);
}
