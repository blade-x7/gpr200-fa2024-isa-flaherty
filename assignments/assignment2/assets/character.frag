#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform float uTime;

void main()
{
    vec2 uv = TexCoord;
    uv *= 4.0f;
    uv.x += uTime;
    uv = fract(uv);
    FragColor =vec4(uv, 0.0, 1.0);

    //Color * abs(tan(uTime));
}