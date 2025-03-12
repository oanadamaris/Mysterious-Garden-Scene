//
//  main.cpp
//  OpenGL Advances Lighting
//
//  Created by CGIS on 28/11/16.
//  Copyright © 2016 CGIS. All rights reserved.
//
#pragma comment(lib,"winmm.lib")
#if defined (__APPLE__)
#define GLFW_INCLUDE_GLCOREARB
#define GL_SILENCE_DEPRECATION
#else
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <Windows.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include "Model3D.hpp"
#include "Camera.hpp"
#include "SkyBox.hpp"
#include <time.h>

#include <iostream>
#include <vector>
#include< mmsystem.h >

#include <GLFW/glfw3.h>

int glWindowWidth = 2869;
int glWindowHeight = 1644;

int retina_width, retina_height;
GLFWwindow* glWindow = NULL;

const unsigned int SHADOW_WIDTH = 4096;
const unsigned int SHADOW_HEIGHT = 4096;

glm::mat4 model;
GLuint modelLoc;
glm::mat4 view;
GLuint viewLoc;
glm::mat4 projection;
GLuint projectionLoc;
glm::mat3 normalMatrix;
GLuint normalMatrixLoc;
glm::mat4 lightRotation;

glm::vec3 lightDir;
GLuint lightDirLoc;
glm::vec3 lightColor;
GLuint lightColorLoc;

gps::Camera myCamera(
	glm::vec3(-0.492111f, 0.955285f, 2.18068f), //position
	glm::vec3(-1.37184f, 0.730154f, -0.07185f), // target
	glm::vec3(0.0f, 1.0f, 0.0f)); //up

float cameraSpeed = 0.01f;

bool pressedKeys[1024];
float angleY = 0.0f;
GLfloat lightAngle;

gps::SkyBox mySkyBox;
gps::Shader skyBoxShader;

gps::Model3D nanosuit;
gps::Model3D ground;
gps::Model3D lightCube;
gps::Model3D screenQuad;
gps::Model3D scene;
gps::Model3D window1;
gps::Model3D window2;
gps::Model3D window3;
gps::Model3D door;
gps::Model3D door_knob;
gps::Model3D door_knob_ext;
gps::Model3D grave_glass;
gps::Model3D garden_light;

gps::Shader myCustomShader;
gps::Shader lightShader;
gps::Shader screenQuadShader;
gps::Shader shaderTransparency;
gps::Shader depthMapShader;

float lightColorX = 1.0f;
float lightColorY = 1.0f;
float lightColorZ = 1.0f;

GLuint shadowMapFBO;
GLuint depthMapTexture;

bool showDepthMap;

bool automatedTourActive = false;
std::vector<glm::vec3> keyPoints;
float t = 0.0f;
int currentPointIndex = 0;
float tourSpeed = 0.005f;

bool directionalLightEnabled = true;
bool pointLightEnabled = true;

bool useDoorKnob = false;

GLuint positionLocPointLight;

struct PointLight {
	glm::vec3 position;
	glm::vec3 color;
	float constant;
	float linear;
	float quadratic;
};

std::vector<PointLight> pointLights(4);

void initPointLights() { ///// X=x, Y=z, Z=-Y

	// lampa cristal greenhouse - usor portocaliu
	pointLights[0].position = glm::vec3(-1.7989f, 0.42908f, -2.1176f);
	pointLights[0].color = glm::vec3(1.0f, 0.7f, 0.4f);
	pointLights[0].constant = 1.0f;
	pointLights[0].linear = 0.35f;
	pointLights[0].quadratic = 0.44;

	// lampa groapa - galben cald
	pointLights[1].position = glm::vec3(3.1793f, 0.12534f, -2.923f);
	pointLights[1].color = glm::vec3(1.0f, 0.917f, 0.0f);
	pointLights[1].constant = 1.0f;
	pointLights[1].linear = 0.35f;
	pointLights[1].quadratic = 0.44f;

	// garden lamp closer to greenhouse
	pointLights[2].position = glm::vec3(-0.128755f, 0.226978f, -0.69f);
	pointLights[2].color = glm::vec3(0.5f, 0.5f, 1.0f);
	pointLights[2].constant = 1.0f;
	pointLights[2].linear = 0.35f;
	pointLights[2].quadratic = 0.44f;

	// garden lamp closer to grave
	pointLights[3].position = glm::vec3(1.67884f, 0.231901f, -2.06333f);
	pointLights[3].color = glm::vec3(0.5f, 0.5f, 1.0f);
	pointLights[3].constant = 1.0f;
	pointLights[3].linear = 0.35f;
	pointLights[3].quadratic = 0.44f;
}

void updatePointLightUniforms() {
	myCustomShader.useShaderProgram();

	for (int i = 0; i < pointLights.size(); i++) {
		std::string number = std::to_string(i);

		glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram,
			("pointLights[" + number + "].position").data()),
			1, glm::value_ptr(pointLights[i].position));

		glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram,
			("pointLights[" + number + "].color").data()),
			1, glm::value_ptr(pointLights[i].color));

		glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram,
			("pointLights[" + number + "].constant").data()),
			pointLights[i].constant);

		glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram,
			("pointLights[" + number + "].linear").data()),
			pointLights[i].linear);

		glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram,
			("pointLights[" + number + "].quadratic").data()),
			pointLights[i].quadratic);
	}

}

long start = clock();
void lightning() {
	long end = clock();
	long dif = end - start;
	if (dif > 3000) {
		if (dif % 20000 >= 0 && dif % 20000 <= 100) {
			PlaySound(TEXT("thunder.wav"), NULL, SND_ASYNC | SND_FILENAME);
			lightColorX = 1.0f;
			lightColorY = 1.0f;
			lightColorZ = 1.0f;
		}
		if (dif % 20000 > 100 && dif % 20000 <= 200) {
			lightColorX = 100.0f;
			lightColorY = 100.0f;
			lightColorZ = 100.0f;

		}
		if (dif % 20000 > 200 && dif % 20000 <= 300) {
			lightColorX = 50.0f;
			lightColorY = 50.0f;
			lightColorZ = 50.0f;
		}
		if (dif % 20000 >= 400 && dif % 20000 <= 500) {
			lightColorX = 0.0f;
			lightColorY = 0.0f;
			lightColorZ = 0.0f;
		}
		if (dif % 20000 >= 600 && dif % 20000 <= 700) {
			lightColorX = 1.0f;
			lightColorY = 1.0f;
			lightColorZ = 1.0f;
		}
		if (dif % 20000 >= 800 && dif % 20000 <= 900) {
			lightColorX = 10.0f;
			lightColorY = 10.0f;
			lightColorZ = 10.0f;
		}
		if (dif % 20000 > 800 && dif % 20000 <= 900) {
			lightColorX = 1.0f;
			lightColorY = 1.0f;
			lightColorZ = 1.0f;
		}
		for (int i = 0; i < 4; i++) {
			pointLights[i].color = glm::vec3(lightColorX, lightColorY, lightColorZ);
		}
	}
}

GLenum glCheckError_(const char* file, int line) {
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);
}

glm::vec3 originalCameraPosition = glm::vec3(-0.492111f, 0.955285f, 2.18068f);
glm::vec3 originalCameraTarget = glm::vec3(-1.37184f, 0.730154f, -0.07185f);

std::vector<glm::vec3> tourPositions = {
	glm::vec3(0.919217f, 0.887842f, 3.576655f),
	glm::vec3(-1.184555f, 0.567842f, 1.297926f),
	glm::vec3(-2.165879f, 0.775601f, -0.794605f),
	glm::vec3(-2.458757f, 0.856954f, -1.921613f),
	glm::vec3(-2.538157f, 1.229610f, -1.694604f),
	glm::vec3(-1.988560f, 1.189392f, 0.831944f),
	glm::vec3(1.730248f, 1.314018f, 0.026358f),
	glm::vec3(1.226500f, 1.167486f, -2.630805f)
};

std::vector<glm::vec3> tourTargets = {
	glm::vec3(0.677295f, 0.887842f, 2.606359f),
	glm::vec3(-1.426477f, 0.567842f, 0.327631f),
	glm::vec3(-2.084512f, 0.417233f, -1.724633f),
	glm::vec3(-2.416503f, -0.017666f, -2.404578f),
	glm::vec3(-1.909699f, 0.452464f, -1.727540f),
	glm::vec3(-1.252888f, 0.831024f, 0.257175f),
	glm::vec3(1.277639f, 0.955650f, -0.790170f),
	glm::vec3(2.078150f, 0.744868f, -2.320829f)
};

void automatedTour() {
	if (currentPointIndex < tourPositions.size() - 1) {
		glm::vec3 p0 = tourPositions[currentPointIndex];
		glm::vec3 p1 = tourPositions[currentPointIndex + 1];
		glm::vec3 currentPos = p0 * (1.0f - t) + p1 * t;

		glm::vec3 target0 = tourTargets[currentPointIndex];
		glm::vec3 target1 = tourTargets[currentPointIndex + 1];
		glm::vec3 currentTarget = target0 * (1.0f - t) + target1 * t;

		myCamera.setPosition(currentPos);
		myCamera.setTarget(currentTarget);

		t += 0.005f;

		if (t >= 1.0f) {
			t = 0.0f;
			currentPointIndex++;

			if (currentPointIndex >= tourPositions.size() - 1) {
				automatedTourActive = false;
				myCamera.setPosition(originalCameraPosition);
				myCamera.setTarget(originalCameraTarget);
				return;
			}
		}
	}
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_M && action == GLFW_PRESS)
		showDepthMap = !showDepthMap;

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			pressedKeys[key] = true;
		if (key == GLFW_KEY_P) {
			automatedTourActive = true;
			currentPointIndex = 0;
			t = 0.0f;
			printf("Automated tour %s\n", automatedTourActive ? "activated" : "deactivated");
		}
		else if (action == GLFW_RELEASE)
			pressedKeys[key] = false;
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
}

void setViewMode(GLenum mode) {
	glPolygonMode(GL_FRONT_AND_BACK, mode);
}

bool isDoorOpening = false;

float doorAngle = 0.0f;
float doorKnobAngle = 0.0f;

float doorSpeed = 0.75f;
float knobSpeed = 0.05f;

bool hasMoved = false;
void processMovement()
{
	float currentSpeed = cameraSpeed;

	if (pressedKeys[GLFW_KEY_LEFT_SHIFT] || pressedKeys[GLFW_KEY_RIGHT_SHIFT]) {
		currentSpeed *= 5.0f;
	}

	if (pressedKeys[GLFW_KEY_UP]) {
		myCamera.move(gps::MOVE_UP, currentSpeed);
		hasMoved = true;
	}

	if (pressedKeys[GLFW_KEY_DOWN]) {
		myCamera.move(gps::MOVE_DOWN, currentSpeed);
		hasMoved = true;
	}

	if (pressedKeys[GLFW_KEY_Q]) {
		myCamera.adjustYaw(-1.0f);
		hasMoved = true;
	}

	if (pressedKeys[GLFW_KEY_E]) {
		myCamera.adjustYaw(+1.0f);
		hasMoved = true;
	}

	if (pressedKeys[GLFW_KEY_J]) {
		lightAngle -= 1.0f;
		lightDir.x = glm::sin(glm::radians(lightAngle));
		lightDir.z = glm::cos(glm::radians(lightAngle));
		myCustomShader.useShaderProgram();
		glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::normalize(lightDir)));
	}

	if (pressedKeys[GLFW_KEY_L]) {
		lightAngle += 1.0f;
		lightDir.x = glm::sin(glm::radians(lightAngle));
		lightDir.z = glm::cos(glm::radians(lightAngle));
		myCustomShader.useShaderProgram();
		glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::normalize(lightDir)));
	}

	if (pressedKeys[GLFW_KEY_W]) {
		myCamera.move(gps::MOVE_FORWARD, currentSpeed);
		hasMoved = true;
	}

	if (pressedKeys[GLFW_KEY_S]) {
		myCamera.move(gps::MOVE_BACKWARD, currentSpeed);
		hasMoved = true;
	}

	if (pressedKeys[GLFW_KEY_LEFT]) {
		myCamera.move(gps::MOVE_LEFT, currentSpeed);
		hasMoved = true;
	}

	if (pressedKeys[GLFW_KEY_RIGHT]) {
		myCamera.move(gps::MOVE_RIGHT, currentSpeed);
		hasMoved = true;
	}

	if (pressedKeys[GLFW_KEY_R]) {
		myCamera.adjustPitch(1.0f);
		hasMoved = true;
	}

	if (pressedKeys[GLFW_KEY_F]) {
		myCamera.adjustPitch(-1.0f);
		hasMoved = true;
	}

	if (pressedKeys[GLFW_KEY_1]) {
		setViewMode(GL_FILL);
	}
	if (pressedKeys[GLFW_KEY_2]) {
		setViewMode(GL_LINE);
	}
	if (pressedKeys[GLFW_KEY_3]) {
		setViewMode(GL_POINT);
	}
	if (pressedKeys[GLFW_KEY_7]) {
		directionalLightEnabled = false;
		pointLightEnabled = false;

		myCustomShader.useShaderProgram();
		glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "directionalLightEnabled"), directionalLightEnabled);
		glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "pointLightEnabled"), pointLightEnabled);
	}

	if (pressedKeys[GLFW_KEY_8]) {
		pointLightEnabled = false;
		directionalLightEnabled = true;

		myCustomShader.useShaderProgram();
		glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "directionalLightEnabled"), directionalLightEnabled);
		glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "pointLightEnabled"), pointLightEnabled);

	}
	if (pressedKeys[GLFW_KEY_9]) {
		directionalLightEnabled = false;
		pointLightEnabled = true;

		myCustomShader.useShaderProgram();
		glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "pointLightEnabled"), pointLightEnabled);
		glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "directionalLightEnabled"), directionalLightEnabled);
	}

	if (hasMoved) {
		glm::vec3 position = myCamera.getPosition();
		glm::vec3 target = myCamera.getTarget();
		printf("Camera Position: (%f, %f, %f) Target: (%f, %f, %f)\n",
			position.x, position.y, position.z,
			target.x, target.y, target.z);
		hasMoved = false;
	}

}

bool initOpenGLWindow()
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

	glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);

	glfwWindowHint(GLFW_SAMPLES, 4);

	glWindow = glfwCreateWindow(glWindowWidth, glWindowHeight, "OpenGL Shader Example", NULL, NULL);
	if (!glWindow) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return false;
	}

	glfwSetWindowSizeCallback(glWindow, windowResizeCallback);
	glfwSetKeyCallback(glWindow, keyboardCallback);
	glfwSetCursorPosCallback(glWindow, mouseCallback);

	glfwMakeContextCurrent(glWindow);

	glfwSwapInterval(1);

#if not defined (__APPLE__)
	glewExperimental = GL_TRUE;
	glewInit();
#endif

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

	return true;
}

void initOpenGLState()
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glViewport(0, 0, retina_width, retina_height);

	glEnable(GL_DEPTH_TEST); 
	glDepthFunc(GL_LESS); 
	//glEnable(GL_CULL_FACE); 
	//glCullFace(GL_BACK); 
	glFrontFace(GL_CCW); 

	glEnable(GL_FRAMEBUFFER_SRGB);
}

void initObjects() {
	scene.LoadModel("glglgl/andSCENE.obj", "glglgl/Textures/");
	window1.LoadModel("glglgl/ROOF_GREENHOUSE.obj", "glglgl/Textures/");
	window2.LoadModel("glglgl/WINDOWS_GREENHOUSE.obj", "glglgl/Textures/");
	window3.LoadModel("glglgl/last_WINDOW_GREENHOUSE.obj", "glglgl/Textures/");
	door.LoadModel("glglgl/door.obj", "glglgl/Textures/");
	door_knob.LoadModel("glglgl/door_knob.obj", "glglgl/Textures/");
	door_knob_ext.LoadModel("glglgl/door_knob_ext.obj", "glglgl/Textures/");
	grave_glass.LoadModel("glglgl/GRAVE_GLASS.obj", "glglgl/Textures/");
	garden_light.LoadModel("glglgl/garden_light.obj", "glglgl/Textures/");
	screenQuad.LoadModel("glglgl/quad/quad.obj");
}

void initShaders() {
	myCustomShader.loadShader("shaders/shaderStart.vert", "shaders/shaderStart.frag");
	myCustomShader.useShaderProgram();
	shaderTransparency.loadShader("shaders/shaderStart.vert", "shaders/shaderTransparency.frag");
	shaderTransparency.useShaderProgram();
	screenQuadShader.loadShader("shaders/screenQuad.vert", "shaders/screenQuad.frag");
	skyBoxShader.loadShader("shaders/skyboxShader.vert", "shaders/skyboxShader.frag");
	skyBoxShader.useShaderProgram();
	depthMapShader.loadShader("shaders/depthMap.vert", "shaders/depthMap.frag");
	depthMapShader.useShaderProgram();
}

void initUniforms() {
	myCustomShader.useShaderProgram();

	model = glm::mat4(1.0f);
	modelLoc = glGetUniformLocation(myCustomShader.shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	view = myCamera.getViewMatrix();
	viewLoc = glGetUniformLocation(myCustomShader.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
	normalMatrixLoc = glGetUniformLocation(myCustomShader.shaderProgram, "normalMatrix");
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	projectionLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	GLuint lightPosLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightPos");
	glm::vec3 lightPos = glm::vec3(3.24877f, 0.0f, -1.73122f);
	glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);

	lightAngle = 90.0f;
	lightDir = glm::vec3(glm::sin(glm::radians(lightAngle)), 0.7f, glm::cos(glm::radians(lightAngle)));
	lightDirLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightDir");
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::normalize(lightDir)));

	
	lightColor = glm::vec3(lightColorX, lightColorY, lightColorZ); 
	lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

}

void initFBO() {

	glGenFramebuffers(1, &shadowMapFBO);
	
	glGenTextures(1, &depthMapTexture);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
		
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::mat4 computeLightSpaceTrMatrix() {
	const GLfloat near_plane = 1.0f, far_plane = 30.0f;
	glm::mat4 lightProjection = glm::ortho(-12.0f, 12.0f, -12.0f, 12.0f, near_plane, far_plane);
	glm::vec3 lightDirNormalized = glm::normalize(lightDir);
	glm::vec3 lightPos = lightDirNormalized * 12.0f;
	glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	return lightProjection * lightView;
}

void drawObjects(gps::Shader shader, bool depthPass) {
	shader.useShaderProgram();

	model = glm::mat4(1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	if (!depthPass) {
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}
	scene.Draw(shader);

	model = glm::mat4(1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	if (!depthPass) {
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}
	grave_glass.Draw(shader);

	model = glm::mat4(1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	if (!depthPass) {
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}
	garden_light.Draw(shader);

}

void renderScene() {

	depthMapShader.useShaderProgram();
	glm::mat4 lightSpaceMatrix = computeLightSpaceTrMatrix();
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "lightSpaceTrMatrix"),
		1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
	
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	drawObjects(depthMapShader, true);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (showDepthMap) {
		glViewport(0, 0, retina_width, retina_height);
		glClear(GL_COLOR_BUFFER_BIT);
		
		screenQuadShader.useShaderProgram();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);
		glUniform1i(glGetUniformLocation(screenQuadShader.shaderProgram, "depthMap"), 0);

		glDisable(GL_DEPTH_TEST);
		screenQuad.Draw(screenQuadShader);
		glEnable(GL_DEPTH_TEST);
	} else {
		
		if (automatedTourActive) {
			automatedTour();
		}
		glViewport(0, 0, retina_width, retina_height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		myCustomShader.useShaderProgram();
		view = glm::rotate(myCamera.getViewMatrix(), glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		lightning();

		lightColor = glm::vec3(lightColorX, lightColorY, lightColorZ);
		lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor");
		glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

		lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);
		glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "shadowMap"), 3);

		glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "lightSpaceTrMatrix"),
			1,
			GL_FALSE,
			glm::value_ptr(lightSpaceMatrix));
		updatePointLightUniforms();
		drawObjects(myCustomShader, false);

		mySkyBox.Draw(skyBoxShader, view, projection);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		shaderTransparency.useShaderProgram();

		glUniformMatrix4fv(glGetUniformLocation(shaderTransparency.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shaderTransparency.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0f);
		glUniformMatrix4fv(glGetUniformLocation(shaderTransparency.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		window1.Draw(shaderTransparency);

		model = glm::mat4(1.0f);
		glUniformMatrix4fv(glGetUniformLocation(shaderTransparency.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		window2.Draw(shaderTransparency);

		model = glm::mat4(1.0f);
		glUniformMatrix4fv(glGetUniformLocation(shaderTransparency.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		window3.Draw(shaderTransparency);

		///////// DOOR + DOOR_KNOBS + ANIMATION /////////
		if (pressedKeys[GLFW_KEY_A]) {
			useDoorKnob = true;
		}

		if (useDoorKnob && !isDoorOpening) {
			if (doorKnobAngle < 35.0f) {
				doorKnobAngle += 0.5f;
			}
			else {
				isDoorOpening = true;
				doorKnobAngle = 0.0f;
				useDoorKnob = false;
			}
		}

		if (isDoorOpening) {
			if (doorAngle > -75.0f) {
				doorAngle -= 1.0f;
			}
		}

		shaderTransparency.useShaderProgram();
		glm::mat4 doorModel = glm::mat4(1.0f);
		doorModel = glm::translate(doorModel, glm::vec3(-2.32372f, -0.010005f, -0.787305f));
		doorModel = glm::rotate(doorModel, glm::radians(doorAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		doorModel = glm::translate(doorModel, glm::vec3(2.32372f, 0.010005f, 0.787305f));
		glUniformMatrix4fv(glGetUniformLocation(shaderTransparency.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(doorModel));
		door.Draw(shaderTransparency);

		myCustomShader.useShaderProgram();
		glm::mat4 knobModel = doorModel;
		if (!isDoorOpening) {
			knobModel = glm::translate(knobModel, glm::vec3(-2.0049f, 0.543524f, -0.966582f));
			knobModel = glm::rotate(knobModel, glm::radians(doorKnobAngle), glm::vec3(0.0f, 0.0f, 1.0f));
			knobModel = glm::translate(knobModel, glm::vec3(2.0049f, -0.543524f, 0.966582f));
		}
		glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(knobModel));
		door_knob.Draw(myCustomShader);

		glm::mat4 knobModel_ext = doorModel;
		if (!isDoorOpening) {

			knobModel_ext = glm::translate(knobModel_ext, glm::vec3(-1.98892f, 0.543524f, -0.930372f));
			knobModel_ext = glm::rotate(knobModel_ext, glm::radians(doorKnobAngle), glm::vec3(0.0f, 0.0f, 1.0f));
			knobModel_ext = glm::translate(knobModel_ext, glm::vec3(1.98892f, -0.543524f, 0.930372f));
		}
		glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(knobModel_ext));
		door_knob_ext.Draw(myCustomShader);
	}
}

void cleanup() {
	glDeleteTextures(1, &depthMapTexture);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &shadowMapFBO);
	glfwDestroyWindow(glWindow);
	glfwTerminate();
}

void initSkyBox() {
	std::vector<const GLchar*> faces;
	faces.push_back("skybox/hills_rt.tga");
	faces.push_back("skybox/hills_lf.tga");
	faces.push_back("skybox/hills_up.tga");
	faces.push_back("skybox/hills_dn.tga");
	faces.push_back("skybox/hills_bk.tga");
	faces.push_back("skybox/hills_ft.tga");
	mySkyBox.Load(faces);
}

int main(int argc, const char* argv[]) {

	if (!initOpenGLWindow()) {
		glfwTerminate();
		return 1;
	}
	initSkyBox();
	initOpenGLState();
	initObjects();
	initShaders();
	initUniforms();
	initPointLights();
	initFBO();

	//glCheckError();

	while (!glfwWindowShouldClose(glWindow)) {
		processMovement();
		renderScene();
		glfwPollEvents();
		glfwSwapBuffers(glWindow);
	}

	cleanup();

	return 0;
}
