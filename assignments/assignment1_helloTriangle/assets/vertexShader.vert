#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
out vec4 Color; //Varying

uniform float uTime;
uniform float uSpeed = 3.0f;

mat4 scale(float x, float y, float z){
	return mat4(
		x, 0.0, 0.0, 0.0, //Col 0
		0.0, y, 0.0, 0.0, //Col 1
		0.0, 0.0, z, 0.0, //Col 2
		0.0, 0.0, 0.0, 1.0 //Col 3
   );
}

//Rotate a radians around z axis
mat4 rotateZ(float a){
	mat4 m = mat4(1); //Identity
	m[0][0] = cos(a);
	m[1][0] = -sin(a);
	m[0][1] = sin(a);
	m[1][1] = cos(a);
	return m;
}

mat4 translate(float x, float y, float z){
	mat4 m = mat4(1);
	m[3][0] = x;
	m[3][1] = y;
	m[3][2] = z;
	return m;
}

void main()
{
   Color = aColor; //Pass-through
   vec4 pos = vec4(aPos, 1.0);
   //pos.y += (sin(uTime * uSpeed + pos.x) * 0.25f);
   
   pos = scale(2.0, 0.5, 1.0) * pos;

   pos = rotateZ(radians(uTime * 180.0)) * pos;

   pos = translate(0.5, 0.5, 0.0) * pos;

   gl_Position = pos; //vec4(pos.x, pos.y, pos.z, 1.0);
}