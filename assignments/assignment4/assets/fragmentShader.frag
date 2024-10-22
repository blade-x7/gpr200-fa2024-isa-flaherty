#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform float uTime;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    
    vec2 uv = TexCoord;
    //uv *= 4.0f;
    //uv.x += uTime;
    //uv = fract(uv);

    vec4 lossCol = texture(texture2, uv);
    FragColor = mix(texture(texture1, uv), texture(texture2, uv), lossCol.a);

    //Color * abs(tan(uTime));
}