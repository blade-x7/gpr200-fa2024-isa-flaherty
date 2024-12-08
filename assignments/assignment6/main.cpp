#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shaderJail/shader.hpp>

#include <ew/external/stb_image.h>

#include <shaderJail/texture.h>
#include <ew/mesh.h>
#include <ew/procGen.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

glm::vec3 cameraPos = glm::vec3(0.0f, 1.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, -5.0f);
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
float fov = 60.0f;

float deltaTime = 0.0f;	
float lastFrame = 0.0f;

bool wireFrame = false;
bool pointRender = false;

float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
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

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	shaderJail::Shader thisShader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	shaderJail::Shader waterShader("assets/waterVertex.vert", "assets/waterFragment.frag");

	//Initialization goes here!
	// IMGUI stuff
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	
	//plane initialization
	ew::MeshData waterMeshData;
	ew::createPlaneXY(10.0f, 10.0f, 11, &waterMeshData);
	ew::Mesh waterMesh = ew::Mesh(waterMeshData);

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//UV
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//block of code here?
	unsigned int tileTexture = shaderJail::loadTexture2D("assets/tiles.png", GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR);
	
	unsigned int loss = shaderJail::loadTexture2D("assets/loss.png", GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //if amongus alpha is zero, is sets the alpha to transparent

	glEnable(GL_DEPTH_TEST);

	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		//Input
		//Update
		float time = (float)glfwGetTime();

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		//Draw
		//Clear framebuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		


		glm::mat4 view = glm::mat4(1.0f);
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		const float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		ew::DrawMode drawMode = pointRender ? ew::DrawMode::POINTS : ew::DrawMode::TRIANGLES;

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

		//water shader stuff
		waterShader.use();

		int viewLocWater = glGetUniformLocation(waterShader.getID(), "view");
		glUniformMatrix4fv(viewLocWater, 1, GL_FALSE, glm::value_ptr(view));

		int projLocWater = glGetUniformLocation(waterShader.getID(), "projection");
		glUniformMatrix4fv(projLocWater, 1, GL_FALSE, glm::value_ptr(projection));

		waterShader.setVec3("lightPos", lightPos);
		waterShader.setVec3("lightColor", lightColor);
		waterShader.setVec3("viewPos", cameraPos);
		waterShader.setFloat("time", time);

		glm::mat4 planeTransform = glm::mat4(1);
		planeTransform = glm::rotate(planeTransform, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		planeTransform = glm::translate(planeTransform, glm::vec3(-5.0, -5.0, 0.0));
		waterShader.setMat4("model", planeTransform);
		waterMesh.draw(drawMode);
		
		
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Settings");
		ImGui::DragFloat3("Light Position", &lightPos.x, 0.1f);
		ImGui::ColorEdit3("Light Color", &lightColor.r);
		if (ImGui::Checkbox("Wireframe", &wireFrame)) {
			glPolygonMode(GL_FRONT_AND_BACK, wireFrame ? GL_LINE : GL_FILL);
		}
		ImGui::End(); 

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	
	float cameraSpeed = 2.5f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		cameraSpeed *= 2;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(glm::cross(cameraFront, cameraUp), cameraFront)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(glm::cross(cameraFront, cameraUp), cameraFront)) * cameraSpeed;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) 
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE) {
		return;
	}

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) 
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 120.0f)
		fov = 120.0f;
}
