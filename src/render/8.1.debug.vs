// vertex shader
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 camPos;
out vec3 v_color;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    camPos = view * worldPos;
    gl_Position = projection * view * worldPos;
    v_color = aColor;
}