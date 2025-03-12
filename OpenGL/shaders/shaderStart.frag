#version 410 core

in vec2 passTexture;
in vec3 fragNormal;
in vec3 fragPosition;
in vec3 fragPositionWorld;
in vec4 fragmentPosEyeSpace;
in vec4 FragPosLightSpace;

out vec4 fragmentColour;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform vec3 lightDir;   
uniform sampler2D shadowMap;

uniform float ambientStrength = 0.2f;
uniform bool directionalLightEnabled;
uniform bool pointLightEnabled;

float specularStrength = 0.5f;
float shininess = 32.0f;
uniform vec3 lightColor;

vec3 viewPos = vec3(0.0f);

#define NR_POINT_LIGHTS 4

struct PointLight {
    vec3 position;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
};

uniform PointLight pointLights[NR_POINT_LIGHTS];

void computeDirLight(out vec3 ambient, out vec3 diffuse, out vec3 specular) {
    vec3 norm = normalize(fragNormal);
    vec3 lightDirNorm = normalize(lightDir);
    vec3 viewDirNorm = normalize(viewPos - fragPosition.xyz);

    ambient = ambientStrength * lightColor;
    diffuse = max(dot(norm, lightDirNorm), 0.0f) * lightColor;
    vec3 reflectDir = normalize(reflect(-lightDirNorm, norm));
    float specCoeff = pow(max(dot(viewDirNorm, reflectDir), 0.0f), shininess);
    specular = specularStrength * specCoeff * lightColor;
}

float ShadowCalculation(vec4 fragPosLightSpace) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    
    if(projCoords.z > 1.0)
        return 0.0;
        
    float currentDepth = projCoords.z;
    
    // Bias bazat pe unghiul dintre normală și direcția luminii
    vec3 normal = normalize(fragNormal);
    vec3 lightDir = normalize(lightDir);
    float cosAngle = max(dot(normal, lightDir), 0.0);
    float bias = max(0.001 * (1.0 - cosAngle), 0.0001);
    
    // PCF simplu cu kernel 3x3
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    
    shadow /= 9.0;
    
    // Atenuare simplă
    return shadow * 0.6; // Intensitate redusă pentru umbre mai subtile
}

vec3 computePointLight(PointLight light) {
    vec3 norm = normalize(fragNormal);
    vec3 lightDirNorm = normalize(light.position - fragPositionWorld);  
    vec3 viewDirNorm = normalize(viewPos - fragPositionWorld);
    
    // Ambient
    vec3 ambient = ambientStrength * light.color;

    // Diffuse
    float diff = max(dot(norm, lightDirNorm), 0.0f);
    vec3 diffuse = diff * light.color;

    // Specular
    vec3 reflectDir = reflect(-lightDirNorm, norm);
    float specCoeff = pow(max(dot(viewDirNorm, reflectDir), 0.0f), shininess);
    vec3 specular = specularStrength * specCoeff * light.color;

    // Attenuation
    float distance = length(light.position - fragPositionWorld);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}

float computeFog() {
    float fogDensity = 0.2f;
    float fragmentDistance = length(fragmentPosEyeSpace);
    float fogFactor = exp(-pow(fragmentDistance * fogDensity, 2));
    return clamp(fogFactor, 0.0f, 1.0f);
}

void main() {
    vec3 light = vec3(0.0);
    vec4 texColor = texture(diffuseTexture, passTexture);

    if (texColor.a < 0.9)
        discard;

    if (directionalLightEnabled) {
        vec3 ambient, diffuse, specular;
        computeDirLight(ambient, diffuse, specular);
        float shadow = ShadowCalculation(FragPosLightSpace);
        light += ambient * 0.4 + (1.0 - shadow) * (diffuse + specular);
    }

    if (pointLightEnabled) {
        for(int i = 0; i < NR_POINT_LIGHTS; i++) {
            light += computePointLight(pointLights[i]);
        }
    }

    float fogFactor = computeFog();
    vec4 fogColor = vec4(0.2f, 0.2f, 0.2f, 1.0f);
    
    fragmentColour = mix(fogColor, vec4(light, 1.0) * texColor, fogFactor);
}
