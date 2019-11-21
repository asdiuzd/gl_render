// fragment shader
#version 330 core

in vec4 camPos;
in vec3 v_color;

out vec4 fragColor;

void main() {
    // float zNear = 0.001;
    // float zFar = 10;
    // float depth = ( 2.0 * zNear ) / ( zFar + zNear - gl_FragCoord.z * ( zFar - zNear ) );
    // fragColor = vec4(depth, depth, depth, 1.f); 
    // float depth = -camPos.z/6;
    fragColor = vec4(v_color.x, v_color.y, v_color.z, 1.f); 
}
