#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform float uTime;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    /*
    vec2 uv = TexCoord;
    uv *= 4.0f;
    uv.x += uTime;
    uv = fract(uv); */

    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);

    //Color * abs(tan(uTime));
}