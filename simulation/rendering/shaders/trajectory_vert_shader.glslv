#version 150

uniform mat4 projection;
uniform mat4 camera;
uniform mat4 model;

uniform vec3 cameraPos;

in vec4 point;
in vec3 tangent;

out float id;
out float cameraFactor;
out vec3 fragVert;

void main() {
    //Passage au fragment shader
    fragVert = vec3(point.y, point.z, point.w);
    id = point.x;
    cameraFactor = pow(1 - abs(dot(normalize(cameraPos - fragVert), normalize(tangent))), 10);

    //Application des modifications de position.
    gl_Position = projection * camera * model * vec4(point.y, point.z, point.w, 1);
}