#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <shaderJail/shader.hpp>

#include <ew/external/stb_image.h>

#include <shaderJail/texture.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;



/*
float vertices[] = {
	// X     Y     Z     R     G     B     A
	-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f
};*/

float vertices[] = {
	// X     Y     Z     U     V
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, //vertex 0
	 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, //vertex 1
	 1.0f,  1.0f, 0.0f, 1.0f, 1.0f, //vertex 2
	-1.0f,  1.0f, 0.0f, 0.0f, 1.0f  //vertex 3
};
unsigned int indices[] = {
	0, 1, 2,
	2, 3, 0
};


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
	shaderJail::Shader charShader("assets/character.vert", "assets/character.frag");

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

	//Element buffer object
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//UV
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//block of code here?
	unsigned int tileTexture = shaderJail::loadTexture2D("assets/tiles.png", GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR);
	
	unsigned int loss = shaderJail::loadTexture2D("assets/loss.png", GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR);

	unsigned int amongusTex = shaderJail::loadTexture2D("assets/amongus.png", GL_CLAMP_TO_EDGE, GL_NEAREST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //if amongus alpha is zero, is sets the alpha to transparent

	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		//Input
		//Update
		float time = (float)glfwGetTime();

		//Draw
		//Clear framebuffer
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT);
		thisShader.use();

		glUniform1i(glGetUniformLocation(thisShader.getID(), "texture1"), 0);
		glUniform1i(glGetUniformLocation(thisShader.getID(), "texture2"), 1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tileTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, loss);


		//glUseProgram(shaderProgram);
		thisShader.setFloat("uTime", time);

		
		//glUniform1f(glGetUniformLocation(shaderProgram, "uTime"), time);

		glBindVertexArray(VAO);

		//Draw call
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//draw amongus
		charShader.use();
		charShader.setFloat("uTime", time);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, amongusTex);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		//Drawing happens here!
		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

unsigned int loadTexture2D(const char* filePath, int wrapMode, int filterMode) {
	stbi_set_flip_vertically_on_load(true);
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (nrChannels == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << stbi_failure_reason() << std::endl;
	}
	stbi_image_free(data);
	return texture;
}
