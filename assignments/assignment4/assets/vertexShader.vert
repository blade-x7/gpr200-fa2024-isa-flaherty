#version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord; //Varying

//uniform float uTime;
//uniform float uSpeed = 3.0f;

void main()
{
   gl_Position = vec4(aPos, 1.0);
   TexCoord = aTexCoord; //Pass-through

   //vec3 pos = aPos;
   //pos.y += (sin(uTime * uSpeed + pos.x) * 0.25f);
}