
#include "main.h"
#include "model.h"
#include "ChessGunman.h"
#include "ChessPawn.h"


//unsigned int loadCubemap(vector<std::string> faces);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(8.0f, 0.0f, 0.0f));
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
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR);

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
	Model EarModel("Chess/Bishop.obj");
	//Chess *bishop_a = new ChessBishop(0, 0, 0, "Chess/Bishop.obj");

	vector <Chess *> chess_list = {
		new ChessKing(0, 0, 0, 0, "Chess/King.obj"),
		new ChessKing(0, 0, 1, 0, "Chess/King.obj"),
		new ChessKing(0, 0, 2, 0, "Chess/King.obj"),
		new ChessQueen(0, 0, 0, 1, "Chess/Queen.obj"),
		new ChessQueen(0, 0, 1, 1, "Chess/Queen.obj"),
		new ChessQueen(0, 0, 2, 1, "Chess/Queen.obj"),
		new ChessRook(0, 0, 0, 2, "Chess/Rook.obj"),
		new ChessRook(0, 0, 1, 2, "Chess/Rook.obj"),
		new ChessRook(0, 0, 2, 2, "Chess/Rook.obj"),
		new ChessBishop(0, 0, 0, 3, "Chess/Bishop.obj"),
		new ChessBishop(0, 0, 1, 3, "Chess/Bishop.obj"),
		new ChessBishop(0, 0, 2, 3, "Chess/Bishop.obj"),
		new ChessBishop(0, 0, 0, 4, "Chess/Bishop.obj"),
		new ChessBishop(0, 0, 1, 4, "Chess/Bishop.obj"),
		new ChessBishop(0, 0, 2, 4, "Chess/Bishop.obj"),
		new ChessKnight(0, 0, 0, 5, "Chess/Knight.obj"),
		new ChessKnight(0, 0, 1, 5, "Chess/Knight.obj"),
		new ChessKnight(0, 0, 2, 5, "Chess/Knight.obj"),
		new ChessKnight(0, 0, 0, 6, "Chess/Knight.obj"),
		new ChessKnight(0, 0, 1, 6, "Chess/Knight.obj"),
		new ChessKnight(0, 0, 2, 6, "Chess/Knight.obj"),
		new ChessViper(0, 0, 0, 7, "Chess/Viper.obj"),
		new ChessViper(0, 0, 1, 7, "Chess/Viper.obj"),
		new ChessViper(0, 0, 2, 7, "Chess/Viper.obj"),
		new ChessWizard(0, 0, 0, 8, "Chess/Wizard.obj"),
		new ChessWizard(0, 0, 1, 8, "Chess/Wizard.obj"),
		new ChessWizard(0, 0, 2, 8, "Chess/Wizard.obj"),
		new ChessGunman(0, 0, 0, 9, "Chess/Gunman.obj"),
		new ChessGunman(0, 0, 1, 9, "Chess/Gunman.obj"),
		new ChessGunman(0, 0, 2, 9, "Chess/Gunman.obj"),
		new ChessPawn(0, 0, 0, 10, "Chess/Pawn.obj"),
		new ChessPawn(0, 0, 1, 10, "Chess/Pawn.obj"),
		new ChessPawn(0, 0, 2, 10, "Chess/Pawn.obj"),
		new ChessPawn(0, 0, 0, 11, "Chess/Pawn.obj"),
		new ChessPawn(0, 0, 1, 11, "Chess/Pawn.obj"),
		new ChessPawn(0, 0, 2, 11, "Chess/Pawn.obj"),
		new ChessPawn(0, 0, 0, 12, "Chess/Pawn.obj"),
		new ChessPawn(0, 0, 1, 12, "Chess/Pawn.obj"),
		new ChessPawn(0, 0, 2, 12, "Chess/Pawn.obj"),
		new ChessPawn(0, 0, 0, 13, "Chess/Pawn.obj"),
		new ChessPawn(0, 0, 1, 13, "Chess/Pawn.obj"),
		new ChessPawn(0, 0, 2, 13, "Chess/Pawn.obj"),
		new ChessPawn(0, 0, 0, 14, "Chess/Pawn.obj"),
		new ChessPawn(0, 0, 1, 14, "Chess/Pawn.obj"),
		new ChessPawn(0, 0, 2, 14, "Chess/Pawn.obj"),
		new ChessPawn(0, 0, 0, 15, "Chess/Pawn.obj"),
		new ChessPawn(0, 0, 1, 15, "Chess/Pawn.obj"),
		new ChessPawn(0, 0, 2, 15, "Chess/Pawn.obj"),
	};



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

	//unsigned int cubemapTexture = loadCubemap(faces);

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

		//sunShader.use();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		//sunShader.setMat4("projection", projection);
		//sunShader.setMat4("view", view);

		show_chess(chess_list);
		
		
		//// don't forget to enable shader before setting uniforms
		//ourShader.use();

		//ourShader.setVec3("objectColor", 0.8f, 0.8f, 0.0f);
		//ourShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		//ourShader.setVec3("lightPos", lightPos);
		//ourShader.setVec3("viewPos", camera.Position);


		//glm::mat4 projection_sun = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		//glm::mat4 view_sun = camera.GetViewMatrix();
		//ourShader.setMat4("projection", projection);
		//ourShader.setMat4("view", view);

		//
		//glm::mat4 model1;    // earth model
		//					 model1 = glm::translate(model1, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		//model1 = glm::scale(model1, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
		//ourShader.setMat4("model", model1);
		////EarModel.Draw(ourShader);
		//(*bishop_a).show(ourShader);
		//
		





		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		// skybox cube
		//glBindVertexArray(skyboxVAO);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glBindVertexArray(0);
		//glDepthFunc(GL_LESS); // set depth function back to default



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



void show_chess(std::vector<Chess *> &chess_list)
{
	Shader our_shader("model.vs", "model.fs");
	// team 1, yellow
	our_shader.use();
	our_shader.setVec3("objectColor", 1.0f, 0.8f, 0.0f);
	our_shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	our_shader.setVec3("lightPos", lightPos);
	our_shader.setVec3("viewPos", camera.Position);


	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();

	// view/projection transformations
	glm::mat4 projection_sun = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view_sun = camera.GetViewMatrix();
	our_shader.setMat4("projection", projection);
	our_shader.setMat4("view", view);
	vector<Chess *>::iterator it = chess_list.begin();
	while(it<chess_list.end())
	{
		glm::mat4 model;
		(*it)->get_model(model);
		our_shader.setMat4("model", model);
		(*it)->show(our_shader);
		++it;
	}

	// team2, green
	our_shader.setVec3("objectColor", 0.0f, 1.0f, 0.0f);
	it = chess_list.begin();
	while (it<chess_list.end())
	{
		glm::mat4 model;
		(*it)->get_model(model);
		our_shader.setMat4("model", model);
		(*it)->show(our_shader);
		++it;
	}

	// team3, red
	our_shader.setVec3("objectColor", 1.0f, 0.2f, 0.1f);
	it = chess_list.begin();
	while (it<chess_list.end())
	{
		glm::mat4 model;
		(*it)->get_model(model);
		our_shader.setMat4("model", model);
		(*it)->show(our_shader);
		++it;
	}
}

// -------------------------------------------------------
//unsigned int loadCubemap(vector<std::string> faces)
//{
//	unsigned int textureID;
//	glGenTextures(1, &textureID);
//	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
//
//	int width, height, nrChannels;
//	for (unsigned int i = 0; i < faces.size(); i++)
//	{
//		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
//		if (data)
//		{
//			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//			stbi_image_free(data);
//		}
//		else
//		{
//			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
//			stbi_image_free(data);
//		}
//	}
//	for (unsigned int i = 0; i < faces.size(); i++)
//	{
//		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
//		if (data)
//		{
//			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//			stbi_image_free(data);
//		}
//		else
//		{
//			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
//			stbi_image_free(data);
//		}
//	}
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//
//	return textureID;
//}