#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{
    
    vec2 uv = TexCoord;
    //uv *= 4.0f;
    //uv.x += uTime;
    //uv = fract(uv);

    float ambientStrength = 0.1;
    float specularStrength = 0.5;

    vec3 ambient = ambientStrength * lightColor;
    
    vec3 objectColor = vec3(0.0, 0.2, 1.0);
    
    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    vec3 reflectDir = reflect(-lightDir, norm); 

    float spec = pow(max(dot(norm, halfwayDir), 0.0), 32);
    vec3 specular = lightColor * spec; 

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}