
#include "main.h"
#include "model.h"
#include <stb_image.h>

#define BLACK glm::vec3(0.001f, 0.0f, 0.0f)
#define WHITE glm::vec3(1.0f, 1.0f, 1.0f)
#define GRAY glm::vec3(0.8f, 0.8f, 0.8f)
#define BLUE glm::vec3(0.0f, 162.0f/256.0f, 232.0f/256.0f)
//#define CHESS_COLOR1 (192.0f/255.0f, 160.0f/255.0f, 170.0f/255.0f)
//#define CHESS_COLOR2 (255.0f/255.0f, 216.0f/255.0f, 169.0f/255.0f)
//#define CHESS_COLOR3 (117.0f/255.0f, 171.0f/255.0f, 242.0f/255.0f)

#define CHESS_COLOR1 (0.0f, 0.0f, 0.0f)
#define CHESS_COLOR2 (0.0f, 0.0f, 0.0f)
#define CHESS_COLOR3 (0.0f, 0.0f, 0.0f)


//unsigned int loadCubemap(vector<std::string> faces);

// settings
#define SCR_WIDTH 1600
#define SCR_HEIGHT 1200

// camera
Camera camera(glm::vec3(0.0f, 2.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
extern bool if_mouse_key_callback = true;
bool mouse_button_status = false;  // if clicked

glm::vec3 lightPos(-4.0f, 4.0f, -1.0f);
vector <ChessBoard *> Block_list;
vector <Chess *> chess_list;

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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Cradle-of-Felicity", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	//glfwSetScrollCallback(window, scroll_callback);

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
	glEnable(GL_STENCIL_TEST);
	//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	//Shader simpleDepthShader("shader/shadow_mapping_depth.vs", "shader/shadow_mapping_depth.fs");
	Shader simpleDepthShader("shader/3.2.2.point_shadows_depth.vs", "shader/3.2.2.point_shadows_depth.fs", "shader/3.2.2.point_shadows_depth.gs");
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

	vector <ChessBoard *> Block_list1 = {
		new ChessBoard("models/Board_Blocks/1.obj", BLACK),
		new ChessBoard("models/Board_Blocks/2.obj", WHITE),
		new ChessBoard("models/Board_Blocks/3.obj", BLACK),
		new ChessBoard("models/Board_Blocks/4.obj", WHITE),
		new ChessBoard("models/Board_Blocks/5.obj", BLACK),
		new ChessBoard("models/Board_Blocks/6.obj", WHITE),
		new ChessBoard("models/Board_Blocks/7.obj", BLACK),
		new ChessBoard("models/Board_Blocks/8.obj", WHITE),
		new ChessBoard("models/Board_Blocks/9.obj", WHITE),
		new ChessBoard("models/Board_Blocks/10.obj", BLACK),
		new ChessBoard("models/Board_Blocks/11.obj", WHITE),
		new ChessBoard("models/Board_Blocks/12.obj", BLACK),
		new ChessBoard("models/Board_Blocks/13.obj", WHITE),
		new ChessBoard("models/Board_Blocks/14.obj", BLACK),
		new ChessBoard("models/Board_Blocks/15.obj", WHITE),
		new ChessBoard("models/Board_Blocks/16.obj", BLACK),
		new ChessBoard("models/Board_Blocks/17.obj", WHITE),
		new ChessBoard("models/Board_Blocks/18.obj", BLACK),
		new ChessBoard("models/Board_Blocks/19.obj", WHITE),
		new ChessBoard("models/Board_Blocks/20.obj", BLACK),
		new ChessBoard("models/Board_Blocks/21.obj", WHITE),
		new ChessBoard("models/Board_Blocks/22.obj", BLACK),
		new ChessBoard("models/Board_Blocks/23.obj", GRAY),
		new ChessBoard("models/Board_Blocks/24.obj", BLACK),
		new ChessBoard("models/Board_Blocks/25.obj", GRAY),
		new ChessBoard("models/Board_Blocks/26.obj", WHITE),
		new ChessBoard("models/Board_Blocks/27.obj", BLACK),
		new ChessBoard("models/Board_Blocks/28.obj", WHITE),
		new ChessBoard("models/Board_Blocks/29.obj", BLACK),
		new ChessBoard("models/Board_Blocks/30.obj", WHITE),
		new ChessBoard("models/Board_Blocks/31.obj", BLACK),
		new ChessBoard("models/Board_Blocks/32.obj", WHITE),
		new ChessBoard("models/Board_Blocks/33.obj", BLACK),
		new ChessBoard("models/Board_Blocks/34.obj", WHITE),
		new ChessBoard("models/Board_Blocks/35.obj", BLUE),
		new ChessBoard("models/Board_Blocks/36.obj", WHITE),
		new ChessBoard("models/Board_Blocks/37.obj", BLUE),
		new ChessBoard("models/Board_Blocks/38.obj", WHITE),
		new ChessBoard("models/Board_Blocks/39.obj", BLUE),
		new ChessBoard("models/Board_Blocks/40.obj", BLACK),
		new ChessBoard("models/Board_Blocks/41.obj", WHITE),
		new ChessBoard("models/Board_Blocks/42.obj", BLACK),
		new ChessBoard("models/Board_Blocks/43.obj", WHITE),
		new ChessBoard("models/Board_Blocks/44.obj", WHITE),
		new ChessBoard("models/Board_Blocks/45.obj", BLACK),
		new ChessBoard("models/Board_Blocks/46.obj", WHITE),
		new ChessBoard("models/Board_Blocks/47.obj", BLACK),
		new ChessBoard("models/Board_Blocks/48.obj", BLUE),
		new ChessBoard("models/Board_Blocks/49.obj", WHITE),
		new ChessBoard("models/Board_Blocks/50.obj", GRAY),
		new ChessBoard("models/Board_Blocks/51.obj", BLACK),
		new ChessBoard("models/Board_Blocks/52.obj", WHITE),
		new ChessBoard("models/Board_Blocks/53.obj", BLUE),
		new ChessBoard("models/Board_Blocks/54.obj", GRAY),
		new ChessBoard("models/Board_Blocks/55.obj", BLACK),
		new ChessBoard("models/Board_Blocks/56.obj", WHITE),
		new ChessBoard("models/Board_Blocks/57.obj", WHITE),
		new ChessBoard("models/Board_Blocks/58.obj", BLACK),
		new ChessBoard("models/Board_Blocks/59.obj", WHITE),
		//new ChessBoard("models/Board_Blocks/60.obj", BLACK)
		new ChessBoard("models/Board_Blocks/61.obj", BLACK),
		new ChessBoard("models/Board_Blocks/62.obj", WHITE),
		new ChessBoard("models/Board_Blocks/63.obj", BLACK),
		new ChessBoard("models/Board_Blocks/64.obj", WHITE),
		new ChessBoard("models/Board_Blocks/65.obj", GRAY),
		new ChessBoard("models/Board_Blocks/66.obj", WHITE),
		new ChessBoard("models/Board_Blocks/67.obj", BLACK),
		new ChessBoard("models/Board_Blocks/68.obj", WHITE),
		new ChessBoard("models/Board_Blocks/69.obj", BLACK),
		new ChessBoard("models/Board_Blocks/70.obj", BLACK),
		new ChessBoard("models/Board_Blocks/71.obj", BLACK),
		new ChessBoard("models/Board_Blocks/72.obj", WHITE),
		new ChessBoard("models/Board_Blocks/73.obj", BLACK),
		new ChessBoard("models/Board_Blocks/74.obj", WHITE),
		new ChessBoard("models/Board_Blocks/75.obj", WHITE),
		new ChessBoard("models/Board_Blocks/76.obj", BLUE),
		new ChessBoard("models/Board_Blocks/77.obj", WHITE),
		new ChessBoard("models/Board_Blocks/78.obj", GRAY),
		new ChessBoard("models/Board_Blocks/79.obj", WHITE),
		new ChessBoard("models/Board_Blocks/80.obj", BLACK),
		new ChessBoard("models/Board_Blocks/81.obj", WHITE),
		new ChessBoard("models/Board_Blocks/82.obj", BLUE),
		new ChessBoard("models/Board_Blocks/83.obj", WHITE),
		new ChessBoard("models/Board_Blocks/84.obj", GRAY),
		new ChessBoard("models/Board_Blocks/85.obj", WHITE),
		new ChessBoard("models/Board_Blocks/86.obj", BLACK),
		new ChessBoard("models/Board_Blocks/87.obj", WHITE),
		new ChessBoard("models/Board_Blocks/88.obj", BLACK),
		new ChessBoard("models/Board_Blocks/89.obj", WHITE),
		new ChessBoard("models/Board_Blocks/90.obj", BLACK),
		new ChessBoard("models/Board_Blocks/91.obj", WHITE),
		new ChessBoard("models/Board_Blocks/92.obj", GRAY),
		new ChessBoard("models/Board_Blocks/93.obj", BLACK),
		new ChessBoard("models/Board_Blocks/94.obj", BLUE),
		new ChessBoard("models/Board_Blocks/95.obj", BLUE),
		new ChessBoard("models/Board_Blocks/96.obj", BLACK),
		new ChessBoard("models/Board_Blocks/97.obj", WHITE),
		new ChessBoard("models/Board_Blocks/98.obj", BLACK),
		new ChessBoard("models/Board_Blocks/99.obj", WHITE),
		new ChessBoard("models/Board_Blocks/100.obj", BLACK),
		new ChessBoard("models/Board_Blocks/101.obj", BLACK),
		new ChessBoard("models/Board_Blocks/102.obj", WHITE),
		new ChessBoard("models/Board_Blocks/103.obj", BLACK),
		new ChessBoard("models/Board_Blocks/104.obj", WHITE),
		new ChessBoard("models/Board_Blocks/105.obj", GRAY),
		new ChessBoard("models/Board_Blocks/106.obj", WHITE),
		new ChessBoard("models/Board_Blocks/107.obj", BLACK),
		new ChessBoard("models/Board_Blocks/108.obj", WHITE),
		new ChessBoard("models/Board_Blocks/109.obj", WHITE),
		new ChessBoard("models/Board_Blocks/110.obj", BLACK),
		new ChessBoard("models/Board_Blocks/111.obj", WHITE),
		new ChessBoard("models/Board_Blocks/112.obj", BLACK),
		new ChessBoard("models/Board_Blocks/113.obj", BLACK),
		new ChessBoard("models/Board_Blocks/114.obj", BLACK),
		new ChessBoard("models/Board_Blocks/115.obj", WHITE),
		new ChessBoard("models/Board_Blocks/116.obj", BLACK),
		new ChessBoard("models/Board_Blocks/117.obj", WHITE),
		new ChessBoard("models/Board_Blocks/118.obj", BLACK),
		new ChessBoard("models/Board_Blocks/119.obj", WHITE),
		new ChessBoard("models/Board_Blocks/120.obj", BLACK),
		new ChessBoard("models/Board_Blocks/121.obj", WHITE),
		new ChessBoard("models/Board_Blocks/122.obj", BLACK),
		new ChessBoard("models/Board_Blocks/123.obj", WHITE),
		new ChessBoard("models/Board_Blocks/124.obj", BLACK),
		new ChessBoard("models/Board_Blocks/125.obj", BLACK)
	};

	Block_list = Block_list1;

	vector <Chess *> chess_list1 = {
		new ChessKing(1, -5, 0, 0, "models/Chess/King1.obj"),
		new ChessKing(7, 3, 1, 0, "models/Chess/King2.obj"),
		new ChessKing(-8, 2, 2, 0, "models/Chess/King3.obj"),
		new ChessQueen(2, -5, 0, 1, "models/Chess/Queen1.obj"),
		new ChessQueen(6, 3, 1, 1, "models/Chess/Queen2.obj"),
		new ChessQueen(-8, 1, 2, 1, "models/Chess/Queen3.obj"),
		new ChessRook(0, -4, 0, 2, "models/Chess/Rook1.obj"),
		new ChessRook(6, 2, 1, 2, "models/Chess/Rook2.obj"),
		new ChessRook(-6, 2, 2, 2, "models/Chess/Rook3.obj"),
		new ChessBishop(-1, -5, 0, 3, "models/Chess/Bishop1.obj"),
		new ChessBishop(8, 2, 1, 3, "models/Chess/Bishop2.obj"),
		new ChessBishop(-6, 4, 2, 3, "models/Chess/Bishop3.obj"),
		new ChessBishop(-3, -5, 0, 4, "models/Chess/Bishop1.obj"),
		new ChessBishop(9, 1, 1, 4, "models/Chess/Bishop2.obj"),
		new ChessBishop(-7, 3, 2, 4, "models/Chess/Bishop3.obj"),
		new ChessKnight(-2, -4, 0, 5, "models/Chess/Knight1.obj"),
		new ChessKnight(7, 1, 1, 5, "models/Chess/Knight2.obj"),
		new ChessKnight(-5, 3, 2, 5, "models/Chess/Knight3.obj"),
		new ChessKnight(2, -4, 0, 6, "models/Chess/Knight1.obj"),
		new ChessKnight(5, 3, 1, 6, "models/Chess/Knight2.obj"),
		new ChessKnight(-7, 1, 2, 6, "models/Chess/Knight3.obj"),
		new ChessViper(4, -4, 0, 7, "models/Chess/Viper1.obj"),
		new ChessViper(4, 4, 1, 7, "models/Chess/Viper2.obj"),
		new ChessViper(-8, 0, 2, 7, "models/Chess/Viper3.obj"),
		new ChessWizard(0, -5, 0, 8, "models/Chess/Wizard1.obj"),
		new ChessWizard(7, 2, 1, 8, "models/Chess/Wizard2.obj"),
		new ChessWizard(-7, 2, 2, 8, "models/Chess/Wizard3.obj"),
		new ChessGunman(-2, -5, 0, 9, "models/Chess/Gunman1.obj", camera),
		new ChessGunman(8, 1, 1, 9, "models/Chess/Gunman2.obj", camera),
		new ChessGunman(-6, 3, 2, 9, "models/Chess/Gunman3.obj", camera),
		new ChessPawn(-5, -4, 0, 10, "models/Chess/Pawn1.obj"),
		new ChessPawn(3, 4, 1, 10, "models/Chess/Pawn2.obj"),
		new ChessPawn(-8, -1, 2, 10, "models/Chess/Pawn3.obj"),
		new ChessPawn(-3, -4, 0, 11, "models/Chess/Pawn1.obj"),
		new ChessPawn(4, 3, 1, 11, "models/Chess/Pawn2.obj"),
		new ChessPawn(-7, 0, 2, 11, "models/Chess/Pawn3.obj"),
		new ChessPawn(-1, -4, 0, 12, "models/Chess/Pawn1.obj"),
		new ChessPawn(5, 2, 1, 12, "models/Chess/Pawn2.obj"),
		new ChessPawn(-6, 1, 2, 12, "models/Chess/Pawn3.obj"),
		new ChessPawn(1, -4, 0, 13, "models/Chess/Pawn1.obj"),
		new ChessPawn(6, 1, 1, 13, "models/Chess/Pawn2.obj"),
		new ChessPawn(-5, 2, 2, 13, "models/Chess/Pawn3.obj"),
		new ChessPawn(3, -4, 0, 14, "models/Chess/Pawn1.obj"),
		new ChessPawn(7, 0, 1, 14, "models/Chess/Pawn2.obj"),
		new ChessPawn(-4, 3, 2, 14, "models/Chess/Pawn3.obj"),
		new ChessPawn(5, -4, 0, 15, "models/Chess/Pawn1.obj"),
		new ChessPawn(8, -1, 1, 15, "models/Chess/Pawn2.obj"),
		new ChessPawn(-3, 4, 2, 15, "models/Chess/Pawn3.obj")
	};

	chess_list = chess_list1;

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
	"models/skybox/right.jpg",
	"models/skybox/left.jpg",
	"models/skybox/top.jpg",
	"models/skybox/bottom.jpg",
	"models/skybox/front.jpg",
	"models/skybox/back.jpg"
	};

	unsigned int cubemap_texture = loadCubemap(faces);



	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);


	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth cubemap texture
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//Shader shader("shader/board-model.vs", "shader/board-model.fs");
	Shader shader("shader/3.2.2.point_shadows.vs", "shader/3.2.2.point_shadows.fs");
	Shader explode_shader("shader/explode.vs", "shader/explode.fs", "shader/explode.gs");


	Shader select_shader("shader/geometry_select.vs", "shader/geometry_select.fs", "shader/geometry_select.gs");

	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix(chess_list);

	explode_shader.use();
	//explode_shader.setInt("diffuseTexture", 0);
	//explode_shader.setInt("shadowMap", 1);
	shader.use();
	shader.setInt("diffuseTexture", 0);
	//shader.setInt("shadowMap", 1);
	shader.setInt("depthMap", 1);

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
		processInput(window, chess_list);

		// render
		// ------
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		process_scene(shader, simpleDepthShader, debugDepthQuad, select_shader, explode_shader, chess_list, Block_list, depthMapFBO, depthMap);


		//glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		//skyboxShader.use();
		//view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		//skyboxShader.setMat4("view", view);
		//skyboxShader.setMat4("projection", projection);
		//glBindVertexArray(skyboxVAO);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_texture);
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


void process_scene(Shader &shader, Shader &DepthShader, Shader &DepthQuad, Shader &select_shader, Shader &explode_shader, std::vector<Chess *> &chess_list, std::vector<ChessBoard *> &block_list, unsigned int &depthMapFBO, unsigned int &depthMap)
{

	//chess_list[23]->if_selected_ = true;

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	const float near_plane = 1.0f;
	const float far_plane = 25.0f;
	const glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), static_cast<float>(SHADOW_WIDTH) / static_cast<float>(SHADOW_HEIGHT), near_plane, far_plane);
	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

	// 1. render scene to depth cubemap
	// --------------------------------
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	DepthShader.use();
	for (unsigned int i = 0; i < 6; ++i)
		DepthShader.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
	DepthShader.setFloat("far_plane", far_plane);
	DepthShader.setVec3("lightPos", lightPos);
	show_chess(chess_list, DepthShader);
	//show_chess_board(chess_board_model, DepthShader);
	show_chess_board(block_list, DepthShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 2. render scene as normal 
	// -------------------------
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	glStencilMask(0x00);
	shader.use();
	const auto projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	const auto view = camera.GetViewMatrix(chess_list);
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	// set lighting uniforms
	shader.setVec3("lightPos", lightPos);
	shader.setVec3("viewPos", camera.Position);
	shader.setInt("shadows", 1); // enable/disable shadows by pressing 'SPACE'
	shader.setFloat("far_plane", far_plane);
	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, woodTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap);

	glStencilMask(0x00);

	show_chess_board(block_list, shader, true);
	//glStencilFunc(GL_ALWAYS, 1, 0xFF);
	//glStencilMask(0xFF);

	show_chess(chess_list, shader, true, false, true);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);


	process_selected(shader, chess_list);

	explode_shader.use();

	explode_shader.setMat4("projection", projection);
	explode_shader.setMat4("view", view);

	show_chess(chess_list, explode_shader, true, true);


	// begin to draw select effect
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	select_shader.use();
	select_shader.setMat4("projection", projection);
	select_shader.setMat4("view", view);
	select_shader.setFloat("time", 0.2);
	for (int i = 0; i < chess_list.size(); i++)
	{
		if (chess_list[i]->_if_explode || chess_list[i]->check_death())
			continue;
		if (!chess_list[i]->if_selected_)
			continue;
		glm::mat4 model = glm::mat4();
		chess_list[i]->get_model(model);
		select_shader.setMat4("model", model);
		if(chess_list[i]->select_effect == 0)
		{
			printf("Error!");
			select_shader.setVec3("objectColor", glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else if (chess_list[i]->select_effect == 1)
			select_shader.setVec3("objectColor", glm::vec3(1.0f, 175.0f/255.0f, 96.0f/255.0f));
		else if (chess_list[i]->select_effect == 2)
			select_shader.setVec3("objectColor", glm::vec3(1.0f, 40.0f / 255.0f, 148.0f / 255.0f));
		chess_list[i]->show(select_shader);
	}


	glBindVertexArray(0);
	glStencilMask(0xFF);
	glEnable(GL_DEPTH_TEST);

	DepthQuad.use();
	DepthQuad.setFloat("near_plane", near_plane);
	DepthQuad.setFloat("far_plane", far_plane);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap);




}





// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window, std::vector<Chess *> chess_list)
{

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (!if_mouse_key_callback)
		return;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		//if_mouse_key_callback = false;
		camera.move_origin_position = glm::vec3(vertice_coordinate[-7 + 9][1 + 5][0], 0.5, vertice_coordinate[-7 + 9][1 + 5][2]);
		//camera.move_destination = glm::vec3(vertice_coordinate[8 + 9][-1 + 5][0], 0.5, vertice_coordinate[8 + 9][-1 + 5][2]);
		camera.move_destination = glm::vec3(vertice_coordinate[2 + 9][-5 + 5][0], 0.5, vertice_coordinate[2 + 9][-5 + 5][2]);

		camera.move_duration = static_cast<int>(sqrt(((pow(camera.move_origin_position.x - camera.move_destination.x, 2) + pow(camera.move_origin_position.z - camera.move_destination.z, 2)))) / init_gun_hor_speed);
		camera.init_gun_vert_speed = 0.5 * G * camera.move_duration;
		printf("vert is %f", camera.init_gun_vert_speed);
		//printf("%f, %f, %f, %f, %f\n", camera.move_origin_position.x, camera.mo)
		printf("move duration is: %d\n", camera.move_duration);
		camera.if_parabola = true;
		camera.If_Move_Auto = true;
		if_mouse_key_callback = false;
		//camera.If_Move_Auto = true;
	}
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
	{
		camera.move_destination = glm::vec3(0.0f, 10.0f, 0.0f);
		camera.move_origin_position = camera.Position;
		camera.destination_sight = glm::vec3(0.0f, 0.0f, 0.0f);
		camera.move_duration = 500;
		camera.If_Move_Auto = true;
		//camera.if_parabola = true;
		if_mouse_key_callback = false;
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		chess_list[5]->get_moving_function(0, 0, camera);
	}
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
	{
		chess_list[23]->get_moving_function(2, 2, camera);
	}
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
	{
		chess_list[14]->get_moving_function(-4, 0, camera);
	}
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
	{
		//chess_list[16]->get_moving_function(1, -1, camera);
		chess_list[27]->get_moving_function(0, 0, camera);
		//chess_list[28]->get_moving_function(0, 0, camera);
	
	}
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
	{
		chess_list[27]->get_moving_function(-2, 0, camera);
	}
	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
	{
		chess_list[27]->get_moving_function(-5, 0, camera);
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		chess_list[27]->get_moving_function(-7, 0, camera);
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		chess_list[27]->get_moving_function(-9, 0, camera);
	}
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
	{
		chess_list[14]->_if_explode = true;
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





void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{;
	if (!if_mouse_key_callback)
		return;
	bool if_process;
	int first_i;
	int first_j;
	int second_i;
	int second_j;
	camera.ProcessMouseMovement(xpos, ypos, chess_list, Block_list, mouse_button_status);
	//printf("status: %d\n", glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT));
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		mouse_button_status = true;
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void process_selected(Shader &shader, std::vector<Chess *> &chess_list)
{
	//vector<Chess *>::iterator it = chess_list.begin();
	glm::vec3 color1 CHESS_COLOR1;
	shader.setVec3("objectColor", color1);
	for (int i = 0; i < chess_list.size(); i+=3)
	{
		if (!chess_list[i]->if_selected_)
		{
			continue;
		}
		glm::mat4 model;
		chess_list[i]->get_model(model);
		shader.setMat4("model", model);
		chess_list[i]->show(shader);
	}
	
	glm::vec3 color2 CHESS_COLOR2;
	shader.setVec3("objectColor", color2);
	for (int i = 1; i < chess_list.size(); i += 3)
	{
		if (!chess_list[i]->if_selected_)
		{
			continue;
		}
		glm::mat4 model;
		chess_list[i]->get_model(model);
		shader.setMat4("model", model);
		chess_list[i]->show(shader);
	}
	glm::vec3 color3 CHESS_COLOR3;
	shader.setVec3("objectColor", color3);
	for (int i = 2; i < chess_list.size(); i += 3)
	{
		if (!chess_list[i]->if_selected_)
		{
			continue;
		}
		glm::mat4 model;
		chess_list[i]->get_model(model);
		shader.setMat4("model", model);
		chess_list[i]->show(shader);
	}
}

void show_chess(std::vector<Chess *> &chess_list, Shader &shader, bool if_real_time, bool if_explode, bool if_motion_blur)
{
	static float counter = 0.1f;
	//shader.setFloat("time", glfwGetTime());
	if (!if_real_time)
	{
		//vector<Chess *>::iterator it = chess_list.begin();
		glm::vec3 color1 CHESS_COLOR1;
		shader.setVec3("objectColor", color1);
		for (int i = 0; i < chess_list.size(); i += 3)
		{
			if (chess_list[i]->check_death())
			{
				continue;
			}
			glm::mat4 model;
			chess_list[i]->get_model(model);
			shader.setMat4("model", model);
			chess_list[i]->show(shader);
		}
		glm::vec3 color2 CHESS_COLOR2;
		shader.setVec3("objectColor", color2);
		for (int i = 1; i < chess_list.size(); i += 3)
		{
			if (chess_list[i]->check_death())
			{
				continue;
			}
			glm::mat4 model;
			chess_list[i]->get_model(model);
			shader.setMat4("model", model);
			chess_list[i]->show(shader);
		}
		glm::vec3 color3 CHESS_COLOR3;
		shader.setVec3("objectColor", color3);
		for (int i = 2; i < chess_list.size(); i += 3)
		{
			if (chess_list[i]->check_death())
			{
				continue;
			}
			glm::mat4 model;
			chess_list[i]->get_model(model);
			shader.setMat4("model", model);
			chess_list[i]->show(shader);
		}	
		return;
	}
	if (!if_explode)
	{
		//vector<Chess *>::iterator it = chess_list.begin();
		glm::vec3 color1 CHESS_COLOR1;
		shader.setVec3("objectColor", color1);
		for (int i = 0; i < chess_list.size(); i += 3)
		{
			if (chess_list[i]->if_selected_ || chess_list[i]->_if_explode || chess_list[i]->check_death())
			{
				continue;
			}
			glm::mat4 model;
			chess_list[i]->get_model(model);
			shader.setMat4("model", model);
			chess_list[i]->show(shader);
		}

		// team2, green
		glm::vec3 color2 CHESS_COLOR2;
		shader.setVec3("objectColor", color2);
		for (int i = 1; i < chess_list.size(); i += 3)
		{
			if (chess_list[i]->if_selected_ || chess_list[i]->_if_explode || chess_list[i]->check_death())
			{
				continue;
			}
			glm::mat4 model;
			chess_list[i]->get_model(model);
			shader.setMat4("model", model);
			chess_list[i]->show(shader);
		}

		// team3, red
		glm::vec3 color3 CHESS_COLOR3;
		shader.setVec3("objectColor", color3);
		for (int i = 2; i < chess_list.size(); i += 3)
		{
			if (chess_list[i]->if_selected_ || chess_list[i]->_if_explode || chess_list[i]->check_death())
			{
				continue;
			}
			glm::mat4 model;
			chess_list[i]->get_model(model);
			shader.setMat4("model", model);
			chess_list[i]->show(shader);
		}
	}
	else
	{
		glm::vec3 color1 CHESS_COLOR1;
		shader.setVec3("objectColor", color1);
		for (int i = 0; i < chess_list.size(); i += 3)
		{
			if (!chess_list[i]->_if_explode || chess_list[i]->check_death())
			{
				continue;
			}
			counter *= 1.4;
			glm::mat4 model;
			chess_list[i]->get_model(model);
			shader.setMat4("model", model);
			if (counter >= 10.0f) {
				counter = 0.01;
				chess_list[i]->_if_explode = false;
				chess_list[i]->set_death();
			}
			shader.setFloat("time", counter);
			chess_list[i]->show(shader);
		}

		// team2, green
		glm::vec3 color2 CHESS_COLOR2;
		shader.setVec3("objectColor", color2);
		for (int i = 1; i < chess_list.size(); i += 3)
		{
			if (!chess_list[i]->_if_explode || chess_list[i]->check_death())
			{
				continue;
			}
			counter *= 1.4;
			glm::mat4 model;
			chess_list[i]->get_model(model);
			shader.setMat4("model", model);
			if (counter >= 10.0f) {
				counter = 0.01;
				chess_list[i]->_if_explode = false;
				chess_list[i]->set_death();
			}
			shader.setFloat("time", counter);
			chess_list[i]->show(shader);
		}

		// team3, red
		glm::vec3 color3 CHESS_COLOR3;
		shader.setVec3("objectColor", color3);
		for (int i = 2; i < chess_list.size(); i += 3)
		{
			if (!chess_list[i]->_if_explode || chess_list[i]->check_death())
			{
				continue;
			}
			counter *= 1.4;
			glm::mat4 model;
			chess_list[i]->get_model(model);
			shader.setMat4("model", model);
			if (counter >= 10.0f) {
				counter = 0.01;
				chess_list[i]->_if_explode = false;
				chess_list[i]->set_death();
			}
			shader.setFloat("time", counter);
			chess_list[i]->show(shader);
		}
		
	}
}


void show_chess_board(std::vector<ChessBoard *> &block_list, Shader &shader, bool if_motion_blur)
{
	//shader.setVec3("objectColor", 0.0f, 0.0f, 0.0f);
	shader.setFloat("time", glfwGetTime());
	vector<ChessBoard *>::iterator it = block_list.begin();
	while (it<block_list.end())
	{
		if (if_motion_blur)
		{
			shader.setInt("num_samples", 10);
			shader.setVec2("velocity", glm::vec2(0.01f, 0.01f));
		}
		else
		{
			shader.setInt("num_samples", 0);
			shader.setVec2("velocity", glm::vec2(0.0f, 0.0f));
		}
		shader.setVec3("objectColor", (*it)->block_color);
		glm::mat4 model;
		(*it)->get_model(model);
		shader.setMat4("model", model);
		(*it)->show(shader);
		//++it;
		it += 1;
	}

}

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