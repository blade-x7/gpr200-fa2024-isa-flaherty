#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform float uTime;

void main()
{
    FragColor =vec4(TexCoord, 0.0, 1.0);

    //Color * abs(tan(uTime));
}