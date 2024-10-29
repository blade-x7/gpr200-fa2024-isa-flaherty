#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
uniform float uTime;
uniform vec3 lightPos;
uniform vec3 lightColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    
    vec2 uv = TexCoord;
    //uv *= 4.0f;
    //uv.x += uTime;
    //uv = fract(uv);

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    
    vec3 objectColor = texture(texture1, TexCoord).rgb;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);

    //Color * abs(tan(uTime));
}