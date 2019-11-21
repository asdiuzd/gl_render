// vertex shader
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 camPos;
out vec4 FragCoord;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    camPos = view * worldPos;
    gl_Position = projection * view * worldPos;
}