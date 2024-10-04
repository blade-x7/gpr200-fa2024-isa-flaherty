#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord; //Varying

uniform float uTime;
uniform float uSpeed = 3.0f;

void main()
{
   TexCoord = aTexCoord; //Pass-through
   vec3 pos = aPos;
   pos /= 2.0;
   pos.y += (sin(uTime * uSpeed) * 0.25f);
   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
}