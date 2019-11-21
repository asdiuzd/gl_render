// fragment shader
#version 330 core
layout (location = 0) out vec3 gAlbedoSpec;

in vec4 camPos;
in vec4 FragCoord;
in vec3 v_color;

out vec4 fragColor;

void main() {
    // gAlbedoSpec = vec3(camPos.x, camPos.y, -camPos.z);
    gAlbedoSpec = v_color;
}
