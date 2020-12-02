#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 vertexColor;

uniform mat4 viewMatrix, projectionMatrix;

out vec4 vColor;

void main(){
    gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.0);
    vColor = vertexColor;
}
