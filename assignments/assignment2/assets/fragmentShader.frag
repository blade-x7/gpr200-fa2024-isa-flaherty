#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
//uniform float uTime;

uniform sampler2D ourTexture;

void main()
{
    //vec2 uv = TexCoord;
    //uv *= 4.0f;
    //uv.x += uTime;
    //uv = fract(uv);

    FragColor = texture(ourTexture, TexCoord);

    //Color * abs(tan(uTime));
}