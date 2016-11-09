#version 150

uniform mat4 projection;
uniform mat4 camera;
uniform mat4 model;

in vec4 point;

out float id;
out vec3 fragVert;

void main() {
    //Passage au fragment shader
    fragVert = vec3(point.x, point.y, point.w);
    id = point.x;

    //Application des modifications de position.
    gl_Position = projection * camera * model * vec4(point.y, point.z, point.w, 1);
}