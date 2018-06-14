#define STB_IMAGE_IMPLEMENTATION
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadCubemap(vector<std::string> faces);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 8.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightPos(0.0f, 0.0f, 0.0f);



int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

														 // glfw window creation
														 // --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Solar System", NULL, NULL);
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

	// build and compile shaders
	// -------------------------
	Shader ourShader("model.vs", "model.fs");
	Shader skyboxShader("starbox.vs", "starbox.fs");
	Shader sunShader("sunmodel.vs", "sunmodel.fs");
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};


	// load models
	// -----------
	Model EarModel("earth/earth.3DS");

	Model MoonModel("moon/moon.3DS");

	Model SaturnoModel("venus/moon.3DS");

	Model SunModel("nanosuit/nanosuit.obj");

	Model Planet1("planet1/moon.3DS");

	Model Planet2("planet2/moon.3DS");


	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	vector<std::string> faces
	{
		"skybox/back.jpg",
		"skybox/back.jpg",
		"skybox/back.jpg",
		"skybox/back.jpg",
		"skybox/back.jpg",
		"skybox/back.jpg"
	};

	unsigned int cubemapTexture = loadCubemap(faces);

	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);


	//Model EarModel("sol/sol.c4d");

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		sunShader.use();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		sunShader.setMat4("projection", projection);
		sunShader.setMat4("view", view);


		glm::mat4 sunmodel;
		//sunmodel = glm::rotate(sunmodel, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		//sunmodel = glm::rotate(sunmodel, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		//sunmodel = glm::scale(sunmodel, glm::vec3(0.002f, 0.002f, 0.002f));	// it's a bit too big for our scene, so scale it down
		sunShader.setMat4("model", sunmodel);
		SunModel.Draw(sunShader);
		
		
		
		// don't forget to enable shader before setting uniforms
		ourShader.use();

		ourShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		ourShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		ourShader.setVec3("lightPos", lightPos);
		ourShader.setVec3("viewPos", camera.Position);


		// view/projection transformations
		glm::mat4 projection_sun = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view_sun = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		

		double EarX = sin(glfwGetTime());
		double EarZ = cos(glfwGetTime());

		// render the loaded model
		glm::mat4 model1;    // earth model
							 //model1 = glm::translate(model1, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model1 = glm::translate(model1, glm::vec3(EarX, 0.0f, EarZ));
		model1 = glm::scale(model1, glm::vec3(0.0002f, 0.0002f, 0.0002f));	// it's a bit too big for our scene, so scale it down
		model1 = glm::rotate(model1, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		model1 = glm::rotate(model1, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		ourShader.setMat4("model", model1);
		EarModel.Draw(ourShader);

		glm::mat4 moonmodel;
		moonmodel = glm::translate(moonmodel, glm::vec3(EarX, 0.0f, EarZ));
		moonmodel = glm::translate(moonmodel, glm::vec3(cos(glfwGetTime() * 4.5) * 0.1, sin(glfwGetTime() *4.5) * 0.1, 0.0f));
		moonmodel = glm::scale(moonmodel, glm::vec3(0.0001f, 0.0001f, 0.0001f));	// it's a bit too big for our scene, so scale it down
		model1 = glm::rotate(model1, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		ourShader.setMat4("model", moonmodel);
		MoonModel.Draw(ourShader);


		double x = glfwGetTime();
		double SatX = sin(x * 0.7);
		double SatY = sin(x * 0.7);
		double SatZ = cos(x * 0.7);
		glm::mat4 saturnomodel;
		saturnomodel = glm::translate(saturnomodel, glm::vec3(SatX, SatY, SatZ));
		saturnomodel = glm::scale(saturnomodel, glm::vec3(0.0005f, 0.0005f, 0.0005f));
		saturnomodel = glm::rotate(saturnomodel, (float)glfwGetTime(), glm::vec3(0.5f, 0.3f, 0.1f));
		ourShader.setMat4("model", saturnomodel);
		SaturnoModel.Draw(ourShader);


		double x1 = glfwGetTime();
		double x2 = x1 + 50;
		double x3 = x2 + 42.3;

		double P1X = 2.1 * (x2 / (pow(x1*x1 + x2 * x2, 0.5)) * cos(x1)) + 2.1 * ((x1 * x3) / (pow(x1*x1 + x2 * x2, 0.5)*pow(x1*x1 + x2 * x2 + x3 * x3, 0.5))) * sin(x1);
		double P1Y = -2.1*x1*cos(x1) / pow(x1*x1 + x2 * x2, 0.5) + 2.1*x2*x3*sin(x1) / (pow(x1*x1 + x2 * x2, 0.5) * pow(x1*x1 + x2 * x2 + x3 * x3, 0.5));
		double P1Z = -2.1*sin(x1) * pow(x1*x1 + x2 * x2, 0.5) / pow(x1*x1 + x2 * x2 + x3 * x3, 0.5);
		glm::mat4 P1model;
		P1model = glm::translate(P1model, glm::vec3(P1X, P1Y, P1Z));
		P1model = glm::scale(P1model, glm::vec3(0.0006f, 0.0006f, 0.0006f));
		P1model = glm::rotate(P1model, (float)glfwGetTime(), glm::vec3(0.5f, 0.3f, 0.1f));
		ourShader.setMat4("model", P1model);
		Planet1.Draw(ourShader);

		x1 = (glfwGetTime() + 93) * 0.3;
		x2 = (x1 + 28) * 0.3;
		x3 = x2 * 0.2;

		P1X = 3.1 * (x2 / (pow(x1*x1 + x2 * x2, 0.5)) * cos(x1)) + 3.1 * ((x1 * x3) / (pow(x1*x1 + x2 * x2, 0.5)*pow(x1*x1 + x2 * x2 + x3 * x3, 0.5))) * sin(x1);
		P1Y = -3.1*x1*cos(x1) / pow(x1*x1 + x2 * x2, 0.5) + 3.1*x2*x3*sin(x1) / (pow(x1*x1 + x2 * x2, 0.5) * pow(x1*x1 + x2 * x2 + x3 * x3, 0.5));
		P1Z = -3.1*sin(x1) * pow(x1*x1 + x2 * x2, 0.5) / pow(x1*x1 + x2 * x2 + x3 * x3, 0.5);
		glm::mat4 P2model;
		P2model = glm::translate(P2model, glm::vec3(P1X, P1Y, P1Z));
		P2model = glm::scale(P2model, glm::vec3(0.0008f, 0.0008f, 0.0008f));
		P2model = glm::rotate(P2model, (float)glfwGetTime(), glm::vec3(0.5f, 0.3f, 0.1f));
		ourShader.setMat4("model", P2model);
		Planet2.Draw(ourShader);


		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default



							  // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
							  // -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVAO);
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
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
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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
	camera.ProcessMouseScroll(yoffset);
}


// -------------------------------------------------------
unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}