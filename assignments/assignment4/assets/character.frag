#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform float uTime;

uniform sampler2D amongus; //0 by default

void main()
{
    vec2 uv = TexCoord;
    FragColor = texture(amongus, uv);

    //Color * abs(tan(uTime));
}