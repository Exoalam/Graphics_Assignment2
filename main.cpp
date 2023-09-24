//
//  main.cpp
//  3D Object Drawing
//
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "table_chair.h"
#include "fan.h"
#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// modelling transform
float rotateAngle_X = 0;
float rotateAngle_Y = 0;
float rotateAngle_Z = 0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;
bool fan_turn = false;
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 1.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

glm::mat4 transforamtion(float tx, float ty, float tz, float rx, float ry, float rz, float sx, float sy, float sz) {
	glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
	translateMatrix = glm::translate(identityMatrix, glm::vec3(tx, ty, tz));
	rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
	rotateYMatrix = glm::rotate(identityMatrix, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
	rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rz), glm::vec3(0.0f, 0.0f, 1.0f));
	scaleMatrix = glm::scale(identityMatrix, glm::vec3(sx, sy, sz));
	model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
	return model;
}

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// ------------------------------------
	Shader ourShader("vertexShader.vs", "fragmentShader.fs");
	//0.59f, 0.19f, 0.0f,
	float table_top[] = {
		0.0f, 0.0f, 0.0f, 0.59f, 0.19f, 0.0f,
		0.5f, 0.0f, 0.0f, 0.59f, 0.19f, 0.0f,
		0.5f, 0.5f, 0.0f, 0.59f, 0.19f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.59f, 0.19f, 0.0f,

		0.5f, 0.0f, 0.0f, 0.59f, 0.19f, 0.0f,
		0.5f, 0.5f, 0.0f, 0.59f, 0.19f, 0.0f,
		0.5f, 0.0f, 0.5f, 0.59f, 0.19f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.59f, 0.19f, 0.0f,

		0.0f, 0.0f, 0.5f, 0.59f, 0.19f, 0.0f,
		0.5f, 0.0f, 0.5f, 0.59f, 0.19f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.59f, 0.19f, 0.0f,
		0.0f, 0.5f, 0.5f, 0.59f, 0.19f, 0.0f,

		0.0f, 0.0f, 0.5f, 0.59f, 0.19f, 0.0f,
		0.0f, 0.5f, 0.5f, 0.59f, 0.19f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.59f, 0.19f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.59f, 0.19f, 0.0f,

		0.5f, 0.5f, 0.5f, 0.59f, 0.19f, 0.0f,
		0.5f, 0.5f, 0.0f, 0.59f, 0.19f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.59f, 0.19f, 0.0f,
		0.0f, 0.5f, 0.5f, 0.59f, 0.19f, 0.0f,

		0.0f, 0.0f, 0.0f, 0.59f, 0.19f, 0.0f,
		0.5f, 0.0f, 0.0f, 0.59f, 0.19f, 0.0f,
		0.5f, 0.0f, 0.5f, 0.59f, 0.19f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.59f, 0.19f, 0.0f
	};
	//0.80f, 0.59f, 0.0f,
	float table_leg[] = {
		0.0f, 0.0f, 0.0f, 0.80f, 0.59f, 0.0f,
		0.5f, 0.0f, 0.0f, 0.80f, 0.59f, 0.0f,
		0.5f, 0.5f, 0.0f, 0.80f, 0.59f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.80f, 0.59f, 0.0f,

		0.5f, 0.0f, 0.0f, 0.80f, 0.59f, 0.0f,
		0.5f, 0.5f, 0.0f, 0.80f, 0.59f, 0.0f,
		0.5f, 0.0f, 0.5f, 0.80f, 0.59f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.80f, 0.59f, 0.0f,

		0.0f, 0.0f, 0.5f, 0.80f, 0.59f, 0.0f,
		0.5f, 0.0f, 0.5f, 0.80f, 0.59f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.80f, 0.59f, 0.0f,
		0.0f, 0.5f, 0.5f, 0.80f, 0.59f, 0.0f,

		0.0f, 0.0f, 0.5f, 0.80f, 0.59f, 0.0f,
		0.0f, 0.5f, 0.5f, 0.80f, 0.59f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.80f, 0.59f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.80f, 0.59f, 0.0f,

		0.5f, 0.5f, 0.5f, 0.80f, 0.59f, 0.0f,
		0.5f, 0.5f, 0.0f, 0.80f, 0.59f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.80f, 0.59f, 0.0f,
		0.0f, 0.5f, 0.5f, 0.80f, 0.59f, 0.0f,

		0.0f, 0.0f, 0.0f, 0.80f, 0.59f, 0.0f,
		0.5f, 0.0f, 0.0f, 0.80f, 0.59f, 0.0f,
		0.5f, 0.0f, 0.5f, 0.80f, 0.59f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.80f, 0.59f, 0.0f
	};
	//0.39f, 0.3f, 0.0f,
	float chair_leg[] = {
		0.0f, 0.0f, 0.0f, 0.39f, 0.3f, 0.0f,
		0.5f, 0.0f, 0.0f, 0.39f, 0.3f, 0.0f,
		0.5f, 0.5f, 0.0f, 0.39f, 0.3f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.39f, 0.3f, 0.0f,

		0.5f, 0.0f, 0.0f, 0.39f, 0.3f, 0.0f,
		0.5f, 0.5f, 0.0f, 0.39f, 0.3f, 0.0f,
		0.5f, 0.0f, 0.5f, 0.39f, 0.3f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.39f, 0.3f, 0.0f,

		0.0f, 0.0f, 0.5f, 0.39f, 0.3f, 0.0f,
		0.5f, 0.0f, 0.5f, 0.39f, 0.3f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.39f, 0.3f, 0.0f,
		0.0f, 0.5f, 0.5f, 0.39f, 0.3f, 0.0f,

		0.0f, 0.0f, 0.5f, 0.39f, 0.3f, 0.0f,
		0.0f, 0.5f, 0.5f, 0.39f, 0.3f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.39f, 0.3f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.39f, 0.3f, 0.0f,

		0.5f, 0.5f, 0.5f, 0.39f, 0.3f, 0.0f,
		0.5f, 0.5f, 0.0f, 0.39f, 0.3f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.39f, 0.3f, 0.0f,
		0.0f, 0.5f, 0.5f, 0.39f, 0.3f, 0.0f,

		0.0f, 0.0f, 0.0f, 0.39f, 0.3f, 0.0f,
		0.5f, 0.0f, 0.0f, 0.39f, 0.3f, 0.0f,
		0.5f, 0.0f, 0.5f, 0.39f, 0.3f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.39f, 0.3f, 0.0f
	};
	//.2f, .2f, .02f,
	float chair_pillar[] = {
		0.0f, 0.0f, 0.0f, .2f, .2f, .02f,
		0.5f, 0.0f, 0.0f, .2f, .2f, .02f,
		0.5f, 0.5f, 0.0f, .2f, .2f, .02f,
		0.0f, 0.5f, 0.0f, .2f, .2f, .02f,

		0.5f, 0.0f, 0.0f, .2f, .2f, .02f,
		0.5f, 0.5f, 0.0f, .2f, .2f, .02f,
		0.5f, 0.0f, 0.5f, .2f, .2f, .02f,
		0.5f, 0.5f, 0.5f, .2f, .2f, .02f,

		0.0f, 0.0f, 0.5f, .2f, .2f, .02f,
		0.5f, 0.0f, 0.5f, .2f, .2f, .02f,
		0.5f, 0.5f, 0.5f, .2f, .2f, .02f,
		0.0f, 0.5f, 0.5f, .2f, .2f, .02f,

		0.0f, 0.0f, 0.5f, .2f, .2f, .02f,
		0.0f, 0.5f, 0.5f, .2f, .2f, .02f,
		0.0f, 0.5f, 0.0f, .2f, .2f, .02f,
		0.0f, 0.0f, 0.0f, .2f, .2f, .02f,

		0.5f, 0.5f, 0.5f, .2f, .2f, .02f,
		0.5f, 0.5f, 0.0f, .2f, .2f, .02f,
		0.0f, 0.5f, 0.0f, .2f, .2f, .02f,
		0.0f, 0.5f, 0.5f, .2f, .2f, .02f,

		0.0f, 0.0f, 0.0f, .2f, .2f, .02f,
		0.5f, 0.0f, 0.0f, .2f, .2f, .02f,
		0.5f, 0.0f, 0.5f, .2f, .2f, .02f,
		0.0f, 0.0f, 0.5f, .2f, .2f, .02f
	};
	//0.9f, 0.9f, 0.0f,
	float chair_back[] = {
		0.0f, 0.0f, 0.0f, 0.9f, 0.9f, 0.0f,
		0.5f, 0.0f, 0.0f, 0.9f, 0.9f, 0.0f,
		0.5f, 0.5f, 0.0f, 0.9f, 0.9f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.9f, 0.9f, 0.0f,

		0.5f, 0.0f, 0.0f, 0.9f, 0.9f, 0.0f,
		0.5f, 0.5f, 0.0f, 0.9f, 0.9f, 0.0f,
		0.5f, 0.0f, 0.5f, 0.9f, 0.9f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.9f, 0.9f, 0.0f,

		0.0f, 0.0f, 0.5f, 0.9f, 0.9f, 0.0f,
		0.5f, 0.0f, 0.5f, 0.9f, 0.9f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.9f, 0.9f, 0.0f,
		0.0f, 0.5f, 0.5f, 0.9f, 0.9f, 0.0f,

		0.0f, 0.0f, 0.5f, 0.9f, 0.9f, 0.0f,
		0.0f, 0.5f, 0.5f, 0.9f, 0.9f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.9f, 0.9f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.9f, 0.9f, 0.0f,

		0.5f, 0.5f, 0.5f, 0.9f, 0.9f, 0.0f,
		0.5f, 0.5f, 0.0f, 0.9f, 0.9f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.9f, 0.9f, 0.0f,
		0.0f, 0.5f, 0.5f, 0.9f, 0.9f, 0.0f,

		0.0f, 0.0f, 0.0f, 0.9f, 0.9f, 0.0f,
		0.5f, 0.0f, 0.0f, 0.9f, 0.9f, 0.0f,
		0.5f, 0.0f, 0.5f, 0.9f, 0.9f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.9f, 0.9f, 0.0f
	};
	//0.69f, 0.69f, 0.69f,
	float floor[] = {
		0.0f, 0.0f, 0.0f, 0.69f, 0.69f, 0.69f,
		0.5f, 0.0f, 0.0f, 0.69f, 0.69f, 0.69f,
		0.5f, 0.5f, 0.0f, 0.69f, 0.69f, 0.69f,
		0.0f, 0.5f, 0.0f, 0.69f, 0.69f, 0.69f,

		0.5f, 0.0f, 0.0f, 0.69f, 0.69f, 0.69f,
		0.5f, 0.5f, 0.0f, 0.69f, 0.69f, 0.69f,
		0.5f, 0.0f, 0.5f, 0.69f, 0.69f, 0.69f,
		0.5f, 0.5f, 0.5f, 0.69f, 0.69f, 0.69f,

		0.0f, 0.0f, 0.5f, 0.69f, 0.69f, 0.69f,
		0.5f, 0.0f, 0.5f, 0.69f, 0.69f, 0.69f,
		0.5f, 0.5f, 0.5f, 0.69f, 0.69f, 0.69f,
		0.0f, 0.5f, 0.5f, 0.69f, 0.69f, 0.69f,

		0.0f, 0.0f, 0.5f, 0.69f, 0.69f, 0.69f,
		0.0f, 0.5f, 0.5f, 0.69f, 0.69f, 0.69f,
		0.0f, 0.5f, 0.0f, 0.69f, 0.69f, 0.69f,
		0.0f, 0.0f, 0.0f, 0.69f, 0.69f, 0.69f,

		0.5f, 0.5f, 0.5f, 0.69f, 0.69f, 0.69f,
		0.5f, 0.5f, 0.0f, 0.69f, 0.69f, 0.69f,
		0.0f, 0.5f, 0.0f, 0.69f, 0.69f, 0.69f,
		0.0f, 0.5f, 0.5f, 0.69f, 0.69f, 0.69f,

		0.0f, 0.0f, 0.0f, 0.69f, 0.69f, 0.69f,
		0.5f, 0.0f, 0.0f, 0.69f, 0.69f, 0.69f,
		0.5f, 0.0f, 0.5f, 0.69f, 0.69f, 0.69f,
		0.0f, 0.0f, 0.5f, 0.69f, 0.69f, 0.69f
	};
	//0.92f, 0.91f, 0.83f,
	float wall1[] = {
		0.0f, 0.0f, 0.0f, 0.92f, 0.91f, 0.83f,
		0.5f, 0.0f, 0.0f, 0.92f, 0.91f, 0.83f,
		0.5f, 0.5f, 0.0f, 0.92f, 0.91f, 0.83f,
		0.0f, 0.5f, 0.0f, 0.92f, 0.91f, 0.83f,

		0.5f, 0.0f, 0.0f, 0.92f, 0.91f, 0.83f,
		0.5f, 0.5f, 0.0f, 0.92f, 0.91f, 0.83f,
		0.5f, 0.0f, 0.5f, 0.92f, 0.91f, 0.83f,
		0.5f, 0.5f, 0.5f, 0.92f, 0.91f, 0.83f,

		0.0f, 0.0f, 0.5f, 0.92f, 0.91f, 0.83f,
		0.5f, 0.0f, 0.5f, 0.92f, 0.91f, 0.83f,
		0.5f, 0.5f, 0.5f, 0.92f, 0.91f, 0.83f,
		0.0f, 0.5f, 0.5f, 0.92f, 0.91f, 0.83f,

		0.0f, 0.0f, 0.5f, 0.92f, 0.91f, 0.83f,
		0.0f, 0.5f, 0.5f, 0.92f, 0.91f, 0.83f,
		0.0f, 0.5f, 0.0f, 0.92f, 0.91f, 0.83f,
		0.0f, 0.0f, 0.0f, 0.92f, 0.91f, 0.83f,

		0.5f, 0.5f, 0.5f, 0.92f, 0.91f, 0.83f,
		0.5f, 0.5f, 0.0f, 0.92f, 0.91f, 0.83f,
		0.0f, 0.5f, 0.0f, 0.92f, 0.91f, 0.83f,
		0.0f, 0.5f, 0.5f, 0.92f, 0.91f, 0.83f,

		0.0f, 0.0f, 0.0f, 0.92f, 0.91f, 0.83f,
		0.5f, 0.0f, 0.0f, 0.92f, 0.91f, 0.83f,
		0.5f, 0.0f, 0.5f, 0.92f, 0.91f, 0.83f,
		0.0f, 0.0f, 0.5f, 0.92f, 0.91f, 0.83f
	};

	//0.99f, 0.84f, 0.70f,
	float wall2[] = {
		0.0f, 0.0f, 0.0f, 0.99f, 0.84f, 0.70f,
		0.5f, 0.0f, 0.0f, 0.99f, 0.84f, 0.70f,
		0.5f, 0.5f, 0.0f, 0.99f, 0.84f, 0.70f,
		0.0f, 0.5f, 0.0f, 0.99f, 0.84f, 0.70f,

		0.5f, 0.0f, 0.0f, 0.99f, 0.84f, 0.70f,
		0.5f, 0.5f, 0.0f, 0.99f, 0.84f, 0.70f,
		0.5f, 0.0f, 0.5f, 0.99f, 0.84f, 0.70f,
		0.5f, 0.5f, 0.5f, 0.99f, 0.84f, 0.70f,

		0.0f, 0.0f, 0.5f, 0.99f, 0.84f, 0.70f,
		0.5f, 0.0f, 0.5f, 0.99f, 0.84f, 0.70f,
		0.5f, 0.5f, 0.5f, 0.99f, 0.84f, 0.70f,
		0.0f, 0.5f, 0.5f, 0.99f, 0.84f, 0.70f,

		0.0f, 0.0f, 0.5f, 0.99f, 0.84f, 0.70f,
		0.0f, 0.5f, 0.5f, 0.99f, 0.84f, 0.70f,
		0.0f, 0.5f, 0.0f, 0.99f, 0.84f, 0.70f,
		0.0f, 0.0f, 0.0f, 0.99f, 0.84f, 0.70f,

		0.5f, 0.5f, 0.5f, 0.99f, 0.84f, 0.70f,
		0.5f, 0.5f, 0.0f, 0.99f, 0.84f, 0.70f,
		0.0f, 0.5f, 0.0f, 0.99f, 0.84f, 0.70f,
		0.0f, 0.5f, 0.5f, 0.99f, 0.84f, 0.70f,

		0.0f, 0.0f, 0.0f, 0.99f, 0.84f, 0.70f,
		0.5f, 0.0f, 0.0f, 0.99f, 0.84f, 0.70f,
		0.5f, 0.0f, 0.5f, 0.99f, 0.84f, 0.70f,
		0.0f, 0.0f, 0.5f, 0.99f, 0.84f, 0.70f
	};



	//0f, 0f, 0f,
	float blackboard[] = {
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,

		0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f
	};

	//0.29f, 0.0f, 0.29f,
	float cabinate[] = {
		0.0f, 0.0f, 0.0f, 0.29f, 0.0f, 0.29f,
		0.5f, 0.0f, 0.0f, 0.29f, 0.0f, 0.29f,
		0.5f, 0.5f, 0.0f, 0.29f, 0.0f, 0.29f,
		0.0f, 0.5f, 0.0f, 0.29f, 0.0f, 0.29f,

		0.5f, 0.0f, 0.0f, 0.29f, 0.0f, 0.29f,
		0.5f, 0.5f, 0.0f, 0.29f, 0.0f, 0.29f,
		0.5f, 0.0f, 0.5f, 0.29f, 0.0f, 0.29f,
		0.5f, 0.5f, 0.5f, 0.29f, 0.0f, 0.29f,

		0.0f, 0.0f, 0.5f, 0.29f, 0.0f, 0.29f,
		0.5f, 0.0f, 0.5f, 0.29f, 0.0f, 0.29f,
		0.5f, 0.5f, 0.5f, 0.29f, 0.0f, 0.29f,
		0.0f, 0.5f, 0.5f, 0.29f, 0.0f, 0.29f,

		0.0f, 0.0f, 0.5f, 0.29f, 0.0f, 0.29f,
		0.0f, 0.5f, 0.5f, 0.29f, 0.0f, 0.29f,
		0.0f, 0.5f, 0.0f, 0.29f, 0.0f, 0.29f,
		0.0f, 0.0f, 0.0f, 0.29f, 0.0f, 0.29f,

		0.5f, 0.5f, 0.5f, 0.29f, 0.0f, 0.29f,
		0.5f, 0.5f, 0.0f, 0.29f, 0.0f, 0.29f,
		0.0f, 0.5f, 0.0f, 0.29f, 0.0f, 0.29f,
		0.0f, 0.5f, 0.5f, 0.29f, 0.0f, 0.29f,

		0.0f, 0.0f, 0.0f, 0.29f, 0.0f, 0.29f,
		0.5f, 0.0f, 0.0f, 0.29f, 0.0f, 0.29f,
		0.5f, 0.0f, 0.5f, 0.29f, 0.0f, 0.29f,
		0.0f, 0.0f, 0.5f, 0.29f, 0.0f, 0.29f
	};
	//0.95f, 0.95f, 0.95f,
	float ceiling[] = {
	0.0f, 0.0f, 0.0f, 0.95f, 0.95f, 0.95f,
	0.5f, 0.0f, 0.0f, 0.95f, 0.95f, 0.95f,
	0.5f, 0.5f, 0.0f, 0.95f, 0.95f, 0.95f,
	0.0f, 0.5f, 0.0f, 0.95f, 0.95f, 0.95f,

	0.5f, 0.0f, 0.0f, 0.95f, 0.95f, 0.95f,
	0.5f, 0.5f, 0.0f, 0.95f, 0.95f, 0.95f,
	0.5f, 0.0f, 0.5f, 0.95f, 0.95f, 0.95f,
	0.5f, 0.5f, 0.5f, 0.95f, 0.95f, 0.95f,

	0.0f, 0.0f, 0.5f, 0.95f, 0.95f, 0.95f,
	0.5f, 0.0f, 0.5f, 0.95f, 0.95f, 0.95f,
	0.5f, 0.5f, 0.5f, 0.95f, 0.95f, 0.95f,
	0.0f, 0.5f, 0.5f, 0.95f, 0.95f, 0.95f,

	0.0f, 0.0f, 0.5f, 0.95f, 0.95f, 0.95f,
	0.0f, 0.5f, 0.5f, 0.95f, 0.95f, 0.95f,
	0.0f, 0.5f, 0.0f, 0.95f, 0.95f, 0.95f,
	0.0f, 0.0f, 0.0f, 0.95f, 0.95f, 0.95f,

	0.5f, 0.5f, 0.5f, 0.95f, 0.95f, 0.95f,
	0.5f, 0.5f, 0.0f, 0.95f, 0.95f, 0.95f,
	0.0f, 0.5f, 0.0f, 0.95f, 0.95f, 0.95f,
	0.0f, 0.5f, 0.5f, 0.95f, 0.95f, 0.95f,

	0.0f, 0.0f, 0.0f, 0.95f, 0.95f, 0.95f,
	0.5f, 0.0f, 0.0f, 0.95f, 0.95f, 0.95f,
	0.5f, 0.0f, 0.5f, 0.95f, 0.95f, 0.95f,
	0.0f, 0.0f, 0.5f, 0.95f, 0.95f, 0.95f
	};

	//1.0f, 1.0f, 1.0f,
	float fan_holder[] = {
		0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,

		0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f
	};
	//.44f, .22f, .05f,
	float fan_pivot[] = {
	0.0f, 0.0f, 0.0f, .44f, .22f, .05f,
	0.5f, 0.0f, 0.0f, .44f, .22f, .05f,
	0.5f, 0.5f, 0.0f, .44f, .22f, .05f,
	0.0f, 0.5f, 0.0f, .44f, .22f, .05f,

	0.5f, 0.0f, 0.0f, .44f, .22f, .05f,
	0.5f, 0.5f, 0.0f, .44f, .22f, .05f,
	0.5f, 0.0f, 0.5f, .44f, .22f, .05f,
	0.5f, 0.5f, 0.5f, .44f, .22f, .05f,

	0.0f, 0.0f, 0.5f, .44f, .22f, .05f,
	0.5f, 0.0f, 0.5f, .44f, .22f, .05f,
	0.5f, 0.5f, 0.5f, .44f, .22f, .05f,
	0.0f, 0.5f, 0.5f, .44f, .22f, .05f,

	0.0f, 0.0f, 0.5f, .44f, .22f, .05f,
	0.0f, 0.5f, 0.5f, .44f, .22f, .05f,
	0.0f, 0.5f, 0.0f, .44f, .22f, .05f,
	0.0f, 0.0f, 0.0f, .44f, .22f, .05f,

	0.5f, 0.5f, 0.5f, .44f, .22f, .05f,
	0.5f, 0.5f, 0.0f, .44f, .22f, .05f,
	0.0f, 0.5f, 0.0f, .44f, .22f, .05f,
	0.0f, 0.5f, 0.5f, .44f, .22f, .05f,

	0.0f, 0.0f, 0.0f, .44f, .22f, .05f,
	0.5f, 0.0f, 0.0f, .44f, .22f, .05f,
	0.5f, 0.0f, 0.5f, .44f, .22f, .05f,
	0.0f, 0.0f, 0.5f, .44f, .22f, .05f
	};
	//.0f, .0f, .42f,
	float fan_blade[] = {
	0.0f, 0.0f, 0.0f, .0f, .0f, .42f,
	0.5f, 0.0f, 0.0f, .0f, .0f, .42f,
	0.5f, 0.5f, 0.0f, .0f, .0f, .42f,
	0.0f, 0.5f, 0.0f, .0f, .0f, .42f,

	0.5f, 0.0f, 0.0f, .0f, .0f, .42f,
	0.5f, 0.5f, 0.0f, .0f, .0f, .42f,
	0.5f, 0.0f, 0.5f, .0f, .0f, .42f,
	0.5f, 0.5f, 0.5f, .0f, .0f, .42f,

	0.0f, 0.0f, 0.5f, .0f, .0f, .42f,
	0.5f, 0.0f, 0.5f, .0f, .0f, .42f,
	0.5f, 0.5f, 0.5f, .0f, .0f, .42f,
	0.0f, 0.5f, 0.5f, .0f, .0f, .42f,

	0.0f, 0.0f, 0.5f, .0f, .0f, .42f,
	0.0f, 0.5f, 0.5f, .0f, .0f, .42f,
	0.0f, 0.5f, 0.0f, .0f, .0f, .42f,
	0.0f, 0.0f, 0.0f, .0f, .0f, .42f,

	0.5f, 0.5f, 0.5f, .0f, .0f, .42f,
	0.5f, 0.5f, 0.0f, .0f, .0f, .42f,
	0.0f, 0.5f, 0.0f, .0f, .0f, .42f,
	0.0f, 0.5f, 0.5f, .0f, .0f, .42f,

	0.0f, 0.0f, 0.0f, .0f, .0f, .42f,
	0.5f, 0.0f, 0.0f, .0f, .0f, .42f,
	0.5f, 0.0f, 0.5f, .0f, .0f, .42f,
	0.0f, 0.0f, 0.5f, .0f, .0f, .42f
	};
	
	unsigned int cube_indices[] = {
		0, 3, 2,
		2, 1, 0,

		4, 5, 7,
		7, 6, 4,

		8, 9, 10,
		10, 11, 8,

		12, 13, 14,
		14, 15, 12,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(table_top), table_top, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	unsigned int VBO2, VAO2, EBO2;
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);
	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(table_leg), table_leg, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	unsigned int VBO3, VAO3, EBO3;
	glGenVertexArrays(1, &VAO3);
	glGenBuffers(1, &VBO3);
	glGenBuffers(1, &EBO3);
	glBindVertexArray(VAO3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(chair_leg), chair_leg, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	unsigned int VBO4, VAO4, EBO4;
	glGenVertexArrays(1, &VAO4);
	glGenBuffers(1, &VBO4);
	glGenBuffers(1, &EBO4);
	glBindVertexArray(VAO4);
	glBindBuffer(GL_ARRAY_BUFFER, VBO4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(chair_pillar), chair_pillar, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO4);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	unsigned int VBO5, VAO5, EBO5;
	glGenVertexArrays(1, &VAO5);
	glGenBuffers(1, &VBO5);
	glGenBuffers(1, &EBO5);
	glBindVertexArray(VAO5);
	glBindBuffer(GL_ARRAY_BUFFER, VBO5);
	glBufferData(GL_ARRAY_BUFFER, sizeof(chair_back), chair_back, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO5);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	unsigned int VBOG, VAOG, EBOG;
	glGenVertexArrays(1, &VAOG);
	glGenBuffers(1, &VBOG);
	glGenBuffers(1, &EBOG);
	glBindVertexArray(VAOG);
	glBindBuffer(GL_ARRAY_BUFFER, VBOG);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floor), floor, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOG);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	unsigned int VBOW1, VAOW1, EBOW1;
	glGenVertexArrays(1, &VAOW1);
	glGenBuffers(1, &VBOW1);
	glGenBuffers(1, &EBOW1);
	glBindVertexArray(VAOW1);
	glBindBuffer(GL_ARRAY_BUFFER, VBOW1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wall1), wall1, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOW1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	unsigned int VBOW2, VAOW2, EBOW2;
	glGenVertexArrays(1, &VAOW2);
	glGenBuffers(1, &VBOW2);
	glGenBuffers(1, &EBOW2);
	glBindVertexArray(VAOW2);
	glBindBuffer(GL_ARRAY_BUFFER, VBOW2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wall2), wall2, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOW2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	unsigned int VBOB, VAOB, EBOB;
	glGenVertexArrays(1, &VAOB);
	glGenBuffers(1, &VBOB);
	glGenBuffers(1, &EBOB);
	glBindVertexArray(VAOB);
	glBindBuffer(GL_ARRAY_BUFFER, VBOB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(blackboard), blackboard, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	unsigned int VBOC, VAOC, EBOC;
	glGenVertexArrays(1, &VAOC);
	glGenBuffers(1, &VBOC);
	glGenBuffers(1, &EBOC);
	glBindVertexArray(VAOC);
	glBindBuffer(GL_ARRAY_BUFFER, VBOC);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cabinate), cabinate, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOC);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	unsigned int VBOT, VAOT, EBOT;
	glGenVertexArrays(1, &VAOT);
	glGenBuffers(1, &VBOT);
	glGenBuffers(1, &EBOT);
	glBindVertexArray(VAOT);
	glBindBuffer(GL_ARRAY_BUFFER, VBOT);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ceiling), ceiling, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOT);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	//Fan
	unsigned int VBOF1, VAOF1, EBOF1;
	glGenVertexArrays(1, &VAOF1);
	glGenBuffers(1, &VBOF1);
	glGenBuffers(1, &EBOF1);
	glBindVertexArray(VAOF1);
	glBindBuffer(GL_ARRAY_BUFFER, VBOF1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fan_holder), fan_holder, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOF1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	unsigned int VBOF2, VAOF2, EBOF2;
	glGenVertexArrays(1, &VAOF2);
	glGenBuffers(1, &VBOF2);
	glGenBuffers(1, &EBOF2);
	glBindVertexArray(VAOF2);
	glBindBuffer(GL_ARRAY_BUFFER, VBOF2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fan_pivot), fan_pivot, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOF2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	unsigned int VBOF3, VAOF3, EBOF3;
	glGenVertexArrays(1, &VAOF3);
	glGenBuffers(1, &VBOF3);
	glGenBuffers(1, &EBOF3);
	glBindVertexArray(VAOF3);
	glBindBuffer(GL_ARRAY_BUFFER, VBOF3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fan_blade), fan_blade, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOF3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);
	int i = 0;
	Table_Chair table_chair[16];
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// activate shader
		ourShader.use();
		glm::mat4 model;
		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		//glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		//glm::mat4 view = basic_camera.createViewMatrix();
		ourShader.setMat4("view", view);
		/*glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/
		//Table_Chair
		float shiftx = -2, shiftz = 0;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				table_chair[i + j].tox = shiftx;
				table_chair[i + j].toz = shiftz;
				ourShader = table_chair[i + j].ret_shader(ourShader, VAO, VAO2, VAO3, VAO4, VAO5);
				shiftz -= 2;
			}
			shiftz = 0;
			shiftx += 2;
		}
		Table_Chair tc;
		tc.tox = 5;
		tc.toz = -8.5;
		ourShader = tc.local_rotation(ourShader, VAO, VAO2, VAO3, VAO4, VAO5, 135);
		//Floor
		model = transforamtion(-2.5, -.8, -9, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 20, 0.1, 24);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOG);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//Wall1
		model = transforamtion(-2.5, -.75, -9, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 20, 7, 0.2);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOW1);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		model = transforamtion(-2.5, -.75, 3, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 20, 7, 0.2);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOW1);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//Wall2
		model = transforamtion(-2.5, -.75, -9, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, .2, 7, 24);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOW2);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		model = transforamtion(7.5, -.75, -9, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, .2, 7, 24);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOW2);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//BlackBoard
		model = transforamtion(-.5, 0.5, -8.9, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 12, 3, 0.2);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOB);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		model = transforamtion(-.6, 0.4, -8.95, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 12.5, 3.5, 0.2);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO4);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//Cabinate
		model = transforamtion(6.75, -.75, -6, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 1.5, 4, 3);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOC);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//Ceiling
		model = transforamtion(-2.5, 2.75, -9, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 20, 0.1, 24);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOT);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//Fan
		model = transforamtion(2, 2.5, -6, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 1, .5, 1);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOF1);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		model = transforamtion(2.125, 2.25, -5.875, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, .5, .75, .5);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOF2);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		Fan fan;
		ourShader = fan.local_rotation(ourShader, VAOF3, i);

		if(fan_turn)
			i+=5;
		// render boxes
		//for (unsigned int i = 0; i < 10; i++)
		//{
		//    // calculate the model matrix for each object and pass it to shader before drawing
		//    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		//    model = glm::translate(model, cubePositions[i]);
		//    float angle = 20.0f * i;
		//    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		//    ourShader.setMat4("model", model);

		//    glDrawArrays(GL_TRIANGLES, 0, 36);
		//}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO2);
	glDeleteVertexArrays(1, &VAO3);
	glDeleteBuffers(1, &VBO3);
	glDeleteBuffers(1, &EBO3);
	glDeleteVertexArrays(1, &VAO4);
	glDeleteBuffers(1, &VBO4);
	glDeleteBuffers(1, &EBO4);
	glDeleteVertexArrays(1, &VAO5);
	glDeleteBuffers(1, &VBO5);
	glDeleteBuffers(1, &EBO5);
	glDeleteVertexArrays(1, &VAOG);
	glDeleteBuffers(1, &VBOG);
	glDeleteBuffers(1, &EBOG);
	glDeleteVertexArrays(1, &VAOW1);
	glDeleteBuffers(1, &VBOW1);
	glDeleteBuffers(1, &EBOW1);
	glDeleteVertexArrays(1, &VAOW2);
	glDeleteBuffers(1, &VBOW2);
	glDeleteBuffers(1, &EBOW2);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		if (!fan_turn) {
			fan_turn = true;
		}
		else {
			fan_turn = false;
		}
	}

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
