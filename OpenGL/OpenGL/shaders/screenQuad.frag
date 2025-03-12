#version 410 core

in vec2 textCoords;
out vec4 fragColor;
uniform sampler2D depthMap;

void main()
{
    float depthValue = texture(depthMap, textCoords).r;
    fragColor = vec4(vec3(depthValue), 1.0);
}
