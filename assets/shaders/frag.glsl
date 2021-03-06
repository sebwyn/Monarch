#version 330 core

in vec2 uv;
in vec4 vColor;
in float tIndex;

out vec4 color;

uniform sampler2D textures[16];
//uniform sampler2D tex;

void main(){
    color = texture(textures[int(round(tIndex))],uv) * vColor;
}