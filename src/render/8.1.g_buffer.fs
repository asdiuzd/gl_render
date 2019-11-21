// fragment shader
#version 330 core
layout (location = 0) out vec3 gAlbedoSpec;

in vec4 camPos;
in vec4 FragCoord;

out vec4 fragColor;

void main() {
    gAlbedoSpec = vec3(camPos.x, camPos.y, -camPos.z);
}
