
#include "main.h"
#include "model.h"



//unsigned int loadCubemap(vector<std::string> faces);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.5f, 1.3f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightPos(-4.0f, 4.0f, -1.0f);

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_SAMPLES, 4);
	//glEnable(GL_MULTISAMPLE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

														 // glfw window creation
														 // --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Cradle-of Grief", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR);

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	Shader simpleDepthShader("shader/shadow_mapping_depth.vs", "shader/shadow_mapping_depth.fs");
	Shader debugDepthQuad("shader/debug_quad.vs", "shader/debug_quad_depth.fs");

	// build and compile shaders
	// -------------------------
	Shader skyboxShader("shader/starbox.vs", "shader/starbox.fs");
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
	//Chess *bishop_a = new ChessBishop(0, 0, 0, "Chess/Bishop.obj");

	Model chess_board_model("models/chessboard/123.obj");


	vector <Chess *> chess_list = {
		new ChessKing(0, 0, 0, 0, "models/Chess/King.obj"),
		new ChessKing(19, -23, 1, 0, "models/Chess/King.obj"),
		new ChessKing(-3, 4, 2, 0, "models/Chess/King.obj"),
		/*new ChessKing(-5, -4, 0, 0, "models/Chess/King.obj"),
		new ChessKing(-4, -4, 1, 0, "models/Chess/King.obj"),
		new ChessKing(-3, -4, 2, 0, "models/Chess/King.obj"),
		new ChessQueen(-4, -4, 0, 1, "models/Chess/Queen.obj"),
		new ChessQueen(0, 0, 1, 1, "models/Chess/Queen.obj"),
		new ChessQueen(0, 0, 2, 1, "models/Chess/Queen.obj"),
		new ChessRook(-3, -4, 0, 2, "models/Chess/Rook.obj"),
		new ChessRook(0, 0, 1, 2, "models/Chess/Rook.obj"),
		new ChessRook(0, 0, 2, 2, "models/Chess/Rook.obj"),*/
		/*new ChessBishop(-2, -7, 0, 3, "models/Chess/Bishop.obj"),
		new ChessBishop(-16, 12, 1, 3, "models/Chess/Bishop.obj"),
		new ChessBishop(12, 4, 2, 3, "models/Chess/Bishop.obj"),
		new ChessBishop(-1, -4, 0, 4, "models/Chess/Bishop.obj"),
		new ChessBishop(0, 0, 1, 4, "models/Chess/Bishop.obj"),
		new ChessBishop(0, 0, 2, 4, "models/Chess/Bishop.obj"),
		new ChessKnight(0, -4, 0, 5, "models/Chess/Knight.obj"),
		new ChessKnight(0, 0, 1, 5, "models/Chess/Knight.obj"),
		new ChessKnight(0, 0, 2, 5, "models/Chess/Knight.obj"),
		new ChessKnight(1, -4, 0, 6, "models/Chess/Knight.obj"),
		new ChessKnight(0, 0, 1, 6, "models/Chess/Knight.obj"),
		new ChessKnight(0, 0, 2, 6, "models/Chess/Knight.obj"),*/
		new ChessViper(2, -4, 0, 7, "models/Chess/Viper.obj"),
		new ChessViper(5, 7, 1, 7, "models/Chess/Viper.obj"),
		new ChessViper(2, 23, 2, 7, "models/Chess/Viper.obj"),
		new ChessWizard(-12, -22, 0, 8, "models/Chess/Wizard.obj"),
		new ChessWizard(-3, 23, 1, 8, "models/Chess/Wizard.obj"),
		new ChessWizard(6, 14, 2, 8, "models/Chess/Wizard.obj"),
		new ChessGunman(9, -16, 0, 9, "models/Chess/Gunman.obj"),
		new ChessGunman(11, 11, 1, 9, "models/Chess/Gunman.obj"),
		new ChessGunman(15, -16, 2, 9, "models/Chess/Gunman.obj"),
		/*new ChessPawn(5, -4, 0, 10, "models/Chess/Pawn.obj"),
		new ChessPawn(0, 0, 1, 10, "models/Chess/Pawn.obj"),
		new ChessPawn(0, 0, 2, 10, "models/Chess/Pawn.obj"),
		new ChessPawn(0, 0, 0, 11, "models/Chess/Pawn.obj"),
		new ChessPawn(0, 0, 1, 11, "models/Chess/Pawn.obj"),
		new ChessPawn(0, 0, 2, 11, "models/Chess/Pawn.obj"),
		new ChessPawn(0, 0, 0, 12, "models/Chess/Pawn.obj"),
		new ChessPawn(0, 0, 1, 12, "models/Chess/Pawn.obj"),
		new ChessPawn(0, 0, 2, 12, "models/Chess/Pawn.obj"),
		new ChessPawn(0, 0, 0, 13, "models/Chess/Pawn.obj"),
		new ChessPawn(0, 0, 1, 13, "models/Chess/Pawn.obj"),
		new ChessPawn(0, 0, 2, 13, "models/Chess/Pawn.obj"),
		new ChessPawn(0, 0, 0, 14, "models/Chess/Pawn.obj"),
		new ChessPawn(0, 0, 1, 14, "models/Chess/Pawn.obj"),
		new ChessPawn(0, 0, 2, 14, "models/Chess/Pawn.obj"),
		new ChessPawn(0, 0, 0, 15, "models/Chess/Pawn.obj"),
		new ChessPawn(0, 0, 1, 15, "models/Chess/Pawn.obj"),
		new ChessPawn(0, 0, 2, 15, "models/Chess/Pawn.obj")*/
	};

	//Model *chessboard = new Model("chessboard/Chessboard.obj");

	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr));


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



	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Shader shader("shader/board-model.vs", "shader/board-model.fs");
	shader.use();
	shader.setInt("diffuseTexture", 0);
	shader.setInt("shadowMap", 1);
	debugDepthQuad.use();
	debugDepthQuad.setInt("depthMap", 0);
	//camera.If_Move_Auto = true;

	//glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);


	//Model EarModel("sol/sol.c4d");

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		const float currentFrame = glfwGetTime();
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
		const glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		//sunShader.setMat4("projection", projection);
		//sunShader.setMat4("view", view);
		
		
		//show_chess(chess_list);

		//show_chess_board(chess_board_model);

		process_scene(shader, simpleDepthShader, debugDepthQuad, chess_list, chess_board_model, depthMapFBO, depthMap);


		//glm::mat4 lightProjection, lightView;
		//glm::mat4 lightSpaceMatrix;
		//float near_plane = 1.0f, far_plane = 7.5f;
		//lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		//lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		//lightSpaceMatrix = lightProjection * lightView;
		//simpleDepthShader.use();
		//simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		//glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		//glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		//glClear(GL_DEPTH_BUFFER_BIT);
		//glActiveTexture(GL_TEXTURE0);
		//show_chess(chess_list, simpleDepthShader);
		//show_chess_board(chess_board_model, simpleDepthShader);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//shader.use();
		//shader.setMat4("projection", projection);
		//shader.setMat4("view", view);
		//shader.setVec3("viewPos", camera.Position);
		//shader.setVec3("lightPos", lightPos);
		//shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		//glActiveTexture(GL_TEXTURE0);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, depthMap);
		//show_chess(chess_list, shader);
		//show_chess_board(chess_board_model, shader);


		//debugDepthQuad.use();
		//debugDepthQuad.setFloat("near_plane", near_plane);
		//debugDepthQuad.setFloat("far_plane", far_plane);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, depthMap);









		

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


void process_scene(Shader &shader, Shader &DepthShader, Shader &DepthQuad, std::vector<Chess *> &chess_list, Model &chess_board_model, unsigned int &depthMapFBO, unsigned int &depthMap)
{
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	static glm::mat4 lightProjection, lightView;
	static glm::mat4 lightSpaceMatrix;
	const float near_plane = 1.0f;
	const float far_plane = 7.5f;
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
	DepthShader.use();
	DepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	show_chess(chess_list, DepthShader);
	show_chess_board(chess_board_model, DepthShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	const glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	shader.use();
	
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	shader.setVec3("viewPos", camera.Position);
	shader.setVec3("lightPos", lightPos);
	shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
	glActiveTexture(GL_TEXTURE0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	show_chess(chess_list, shader);
	show_chess_board(chess_board_model, shader);


	DepthQuad.use();
	DepthQuad.setFloat("near_plane", near_plane);
	DepthQuad.setFloat("far_plane", far_plane);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap);

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
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		camera.If_Move_Auto = true;
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
//void mouse_callback(GLFWwindow* window, double xpos, double ypos)
//{
//	if (firstMouse)
//	{
//		lastX = xpos;
//		lastY = ypos;
//		firstMouse = false;
//	}
//
//	float xoffset = xpos - lastX;
//	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
//
//	lastX = xpos;
//	lastY = ypos;
//
//	camera.ProcessMouseMovement(xoffset, yoffset);
//}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}



void show_chess(std::vector<Chess *> &chess_list, Shader &shader)
{
	//Shader our_shader("shader/model.vs", "shader/model.fs");
	//// team 1, yellow
	//our_shader.use();
	//our_shader.setVec3("objectColor", 1.0f, 0.8f, 0.0f);
	//our_shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	//our_shader.setVec3("lightPos", lightPos);
	//our_shader.setVec3("viewPos", camera.Position);


	//const glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 100.0f);
	//const glm::mat4 view = camera.GetViewMatrix();

	//// view/projection transformations

	//our_shader.setMat4("projection", projection);
	//our_shader.setMat4("view", view);
	shader.setVec3("objectColor", 1.0f, 0.8f, 0.0f);
	vector<Chess *>::iterator it = chess_list.begin();
	while(it<chess_list.end())
	{
		glm::mat4 model;
		(*it)->get_model(model);
		shader.setMat4("model", model);
		(*it)->show(shader);
		//++it;
		it += 3;
	}

	// team2, green
	shader.setVec3("objectColor", 0.0f, 1.0f, 0.0f);
	it = chess_list.begin();
	++it;
	while (it<chess_list.end())
	{
		glm::mat4 model;
		(*it)->get_model(model);
		shader.setMat4("model", model);
		(*it)->show(shader);
		it += 3;
	}

	// team3, red
	shader.setVec3("objectColor", 1.0f, 0.2f, 0.1f);
	it = chess_list.begin();
	it += 2;
	while (it<chess_list.end())
	{
		glm::mat4 model;
		(*it)->get_model(model);
		shader.setMat4("model", model);
		(*it)->show(shader);
		it += 3;
	}
}

void show_chess_board(Model &chess_board_model, Shader &shader)
{
	//Shader our_shader("shader/board-model.vs", "shader/board-model.fs");
	//our_shader.use();
	//const auto projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 100.0f);
	//const auto view = camera.GetViewMatrix();

	// view/projection transformations

	//our_shader.setMat4("projection", projection);
	//our_shader.setMat4("view", view);
	shader.setVec3("objectColor", 0.0f, 0.0f, 0.0f);
	glm::mat4 board_model;
	board_model = glm::rotate(board_model, 3.14f/2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	//sunmodel = glm::rotate(sunmodel, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	board_model = glm::scale(board_model, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
	shader.setMat4("model", board_model);
	chess_board_model.Draw(shader);
	
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