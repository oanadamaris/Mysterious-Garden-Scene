#version 410 core

uniform sampler2D diffuseTexture;
in vec2 passTexture;

void main() {
    vec4 texColor = texture(diffuseTexture, passTexture);
    if(texColor.a < 0.9)
        discard;
} 