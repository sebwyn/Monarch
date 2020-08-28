#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float texID;

uniform mat4 viewMatrix, projectionMatrix;

out vec2 uv;
out vec4 vColor;
out float tIndex;

void main(){
    gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.0);
    uv = texCoord;
    vColor = vertexColor;
    tIndex = texID;
}
