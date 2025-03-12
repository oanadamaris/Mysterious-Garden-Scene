#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;
layout(location=2) in vec2 vTexCoords;

out vec3 fragPosition;
out vec3 fragNormal;
out vec2 passTexture;
out vec3 fragPositionWorld;
out vec4 fragmentPosEyeSpace;
out vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceTrMatrix;

void main() {
    fragPosition = vPosition;
    fragNormal = vNormal;
    passTexture = vTexCoords;
    fragPositionWorld = (model * vec4(vPosition, 1.0f)).xyz;
    fragmentPosEyeSpace = view * model * vec4(vPosition, 1.0f);
    FragPosLightSpace = lightSpaceTrMatrix * model * vec4(vPosition, 1.0f);
    gl_Position = projection * view * model * vec4(vPosition, 1.0f);
}
