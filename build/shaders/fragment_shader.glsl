#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec3 ObjectColor;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

// Ambient light intensity
uniform float ambientStrength;
// Diffuse light intensity
uniform float diffuseStrength;
// Specular light intensity
uniform float specularStrength;
// Shininess factor
uniform float shininess;

void main()
{
    // === PHONG LIGHTING MODEL ===
    
    // 1. Ambient Lighting (pencahayaan dasar)
    vec3 ambient = ambientStrength * lightColor;
    
    // 2. Diffuse Lighting (pencahayaan berdasarkan sudut permukaan)
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * diff * lightColor;
    
    // 3. Specular Lighting (pantulan cahaya)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;
    
    // Gabungkan semua komponen pencahayaan
    vec3 result = (ambient + diffuse + specular) * ObjectColor;
    FragColor = vec4(result, 1.0);
}
