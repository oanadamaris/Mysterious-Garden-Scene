#version 410 core

in vec2 passTexture; 
out vec4 fragmentColour; 
uniform sampler2D diffuseTexture; 
void main() {
  
    vec4 texColor = texture(diffuseTexture, passTexture);

       fragmentColour = vec4(texColor.rgb, 0.1); 
}