#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <shaderJail/shader.hpp>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;



float vertices[] = {
	// X     Y     Z     R     G     B     A
	-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f
};

/*
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
out vec4 Color; //Varying

uniform float uTime;
uniform float uSpeed = 3.0f;

void main()
{
   Color = aColor; //Pass-through
   vec3 pos = aPos;
   pos.y += (sin(uTime * uSpeed + pos.x) * 0.25f);
   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
})";*/

/*
const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
in vec4 Color;
uniform float uTime;

void main()
{
    FragColor = Color * abs(tan(uTime));
} 
)";*/

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	shaderJail::Shader thisShader("assets/vertexShader.vert", "assets/fragmentShader.frag");

	//Initialization goes here!
	//Vertex array object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	//create vertex buffer object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//put data into the currently bound VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	

	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		//Input
		//Update
		float time = (float)glfwGetTime();

		//Draw
		//Clear framebuffer
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		thisShader.use();
		//glUseProgram(shaderProgram);
		thisShader.setFloat("uTime", time);
		//glUniform1f(glGetUniformLocation(shaderProgram, "uTime"), time);

		glBindVertexArray(VAO);

		//Draw call
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//Drawing happens here!
		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}
