
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
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 2.0f, 3.0f));
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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Cradle-of-Felicity", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//glfwSetCursorPosCallback(window, mouse_callback);
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
	Shader simpleDepthShader("3.2.2.point_shadows_depth.vs", "3.2.2.point_shadows_depth.fs", "3.2.2.point_shadows_depth.gs");
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

	//Model chess_board_model("models/chessboard/123.obj");

	vector <ChessBoard *> Block_list = {
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

	vector <Chess *> chess_list = {
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
		new ChessGunman(-2, -5, 0, 9, "models/Chess/Gunman1.obj"),
		new ChessGunman(8, 1, 1, 9, "models/Chess/Gunman2.obj"),
		new ChessGunman(-6, 3, 2, 9, "models/Chess/Gunman3.obj"),
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
		"models/Board_Blocks/white.png",
		"models/Board_Blocks/white.png",
		"models/Board_Blocks/white.png",
		"models/Board_Blocks/white.png",
		"models/Board_Blocks/white.png",
		"models/Board_Blocks/white.png"
	};

	unsigned int cubemapTexture = loadCubemap(faces);



	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);



	//unsigned int depthMapFBO;
	//glGenFramebuffers(1, &depthMapFBO);
	//// create depth texture
	//unsigned int depthMap;
	//glGenTextures(1, &depthMap);
	//glBindTexture(GL_TEXTURE_2D, depthMap);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	//float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	//// attach depth texture as FBO's depth buffer
	//glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
	Shader shader("3.2.2.point_shadows.vs", "3.2.2.point_shadows.fs");
	Shader explode_shader("shader/explode.vs", "shader/explode.fs", "shader/explode.gs");


	Shader shaderSingleColor("shader/2.stencil_testing.vs", "shader/2.stencil_single_color.fs");

	Shader select_shader("shader/geometry_select.vs", "shader/geometry_select.fs", "shader/geometry_select.gs");

	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();

	shaderSingleColor.use();

	shaderSingleColor.setMat4("view", view);
	shaderSingleColor.setMat4("projection", projection);


	explode_shader.use();
	//explode_shader.setInt("diffuseTexture", 0);
	//explode_shader.setInt("shadowMap", 1);
	shader.use();
	shader.setInt("diffuseTexture", 0);
	//shader.setInt("shadowMap", 1);
	shader.setInt("depthMap", 1);
	//debugDepthQuad.use();
	//debugDepthQuad.setInt("depthMap", 0);


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
		processInput(window, chess_list);

		// render
		// ------
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//sunShader.use();

		//sunShader.setMat4("projection", projection);
		//sunShader.setMat4("view", view);


		//show_chess(chess_list);

		//show_chess_board(chess_board_model);

		//process_scene(shader, simpleDepthShader, debugDepthQuad, chess_list, chess_board_model, depthMapFBO, depthMap);


		//glStencilFunc(GL_ALWAYS, 1, 0xFF);
		//glStencilMask(0xFF);
		process_scene(shader, simpleDepthShader, debugDepthQuad, select_shader, explode_shader, chess_list, Block_list, depthMapFBO, depthMap);

		/*shaderSingleColor.use();
		float scale = 1.1;
		const glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		shaderSingleColor.setMat4("view", view);
		shaderSingleColor.setMat4("projection", projection);

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		shaderSingleColor.use();

		shaderSingleColor.use();
		glm::mat4 model1;
		chess_list[0]->get_model(model1);
		model1 = glm::scale(model1, glm::vec3(scale, scale, scale));
		shaderSingleColor.setMat4("model", model1);
		chess_list[0]->chess_model_.Draw(shaderSingleColor);
		glBindVertexArray(0);
		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);*/




		//glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		//skyboxShader.use();
		//view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		//skyboxShader.setMat4("view", view);
		//skyboxShader.setMat4("projection", projection);
		//// skybox cube
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


void process_scene(Shader &shader, Shader &DepthShader, Shader &DepthQuad, Shader &select_shader, Shader &explode_shader, std::vector<Chess *> &chess_list, std::vector<ChessBoard *> &block_list, unsigned int &depthMapFBO, unsigned int &depthMap)
{

	chess_list[23]->if_selected_ = true;

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	float near_plane = 1.0f;
	float far_plane = 25.0f;
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
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
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
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

	//renderScene(shader);

	glStencilMask(0x00);

	show_chess_board(block_list, shader, true);
	//glStencilFunc(GL_ALWAYS, 1, 0xFF);
	//glStencilMask(0xFF);

	show_chess(chess_list, shader, true, false, true);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);



	/*static glm::mat4 lightProjection, lightView;
	static glm::mat4 lightSpaceMatrix;
	const float near_plane = 1.0f;
	const float far_plane = 10.5f;
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
	DepthShader.use();
	DepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	show_chess(chess_list, DepthShader);
	show_chess_board(block_list, DepthShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glStencilMask(0x00);

	shader.use();

	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	shader.setVec3("viewPos", camera.Position);
	shader.setVec3("lightPos", lightPos);
	shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
	glActiveTexture(GL_TEXTURE0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	glStencilMask(0x00);

	show_chess_board(block_list, shader);


	show_chess(chess_list, shader, true);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);*/

	process_selected(shader, chess_list);

	explode_shader.use();

	explode_shader.setMat4("projection", projection);
	explode_shader.setMat4("view", view);
	//explode_shader.setVec3("viewPos", camera.Position);
	//explode_shader.setVec3("lightPos", lightPos);
	//explode_shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

	show_chess(chess_list, explode_shader, true, true);

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	select_shader.use();
	select_shader.setMat4("projection", projection);
	select_shader.setMat4("view", view);
	glm::mat4 model = glm::mat4();
	chess_list[23]->get_model(model);
	select_shader.setMat4("model", model);
	select_shader.setFloat("time", 0.2);
	chess_list[23]->chess_model_.Draw(select_shader);


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
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
	{
		camera.move_destination = glm::vec3(0.0f, 10.0f, 0.0f);
		camera.move_origin_position = camera.Position;
		camera.destination_sight = glm::vec3(0.0f, 0.0f, 0.0f);
		camera.move_duration = 500;
		camera.If_Move_Auto = true;
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		chess_list[5]->get_moving_function(0, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
	{
		chess_list[23]->get_moving_function(2, 2);
	}
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
	{
		chess_list[14]->get_moving_function(-4, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
	{
		chess_list[16]->get_moving_function(1, -1);
	}
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
	{
		chess_list[21]->get_moving_function(4, -1);
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		chess_list[5]->_if_explode = true;
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		chess_list[10]->_if_explode = true;
	}
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
	{
		chess_list[15]->_if_explode = true;
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





void mouse_callback(GLFWwindow* window, double xpos, double ypos, std::vector<Chess *>chess_list)
{
	//if (firstMouse)
	//{
	//	lastX = xpos;
	//	lastY = ypos;
	//	firstMouse = false;
	//}

	//float xoffset = xpos - lastX;
	//float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	//lastX = xpos;
	//lastY = ypos;

	camera.ProcessMouseMovement(xpos, ypos, chess_list);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void process_selected(Shader &shader, std::vector<Chess *> &chess_list)
{
	vector<Chess *>::iterator it = chess_list.begin();
	glm::vec3 color1 CHESS_COLOR1;
	shader.setVec3("objectColor", color1);
	while (it<chess_list.end())
	{
		if (!(*it)->if_selected_) {
			it += 3;
			continue;
		}
		// team1, yellow
		glm::mat4 model;
		(*it)->get_model(model);
		shader.setMat4("model", model);
		(*it)->show(shader);
		//++it;
		it += 3;
	}

	// team2, blue
	glm::vec3 color2 CHESS_COLOR2;
	shader.setVec3("objectColor", color2);
	it = chess_list.begin();
	++it;
	while (it<chess_list.end())
	{
		if (!(*it)->if_selected_) {
			it += 3;
			continue;
		}
		glm::mat4 model;
		(*it)->get_model(model);
		shader.setMat4("model", model);
		(*it)->show(shader);
		//++it;
		it += 3;
	}

	// team3, red
	glm::vec3 color3 CHESS_COLOR3;
	shader.setVec3("objectColor", color3);
	it = chess_list.begin();
	it += 2;
	while (it<chess_list.end())
	{
		if (!(*it)->if_selected_) {
			it += 3;
			continue;
		}
		glm::mat4 model;
		(*it)->get_model(model);
		shader.setMat4("model", model);
		(*it)->show(shader);
		//++it;
		it += 3;
	}
	return;
}

void show_chess(std::vector<Chess *> &chess_list, Shader &shader, bool if_real_time, bool if_explode, bool if_motion_blur)
{
	static float counter = 0.1f;
	//shader.setFloat("time", glfwGetTime());
	if (!if_real_time)
	{
		vector<Chess *>::iterator it = chess_list.begin();
		glm::vec3 color1 CHESS_COLOR1;
		shader.setVec3("objectColor", color1);
		while (it<chess_list.end())
		{
			if ((*it)->check_death())
			{
				it += 3;
				continue;
			}
			// team1, yellow

			glm::mat4 model;
			(*it)->get_model(model);
			shader.setMat4("model", model);
			(*it)->show(shader);
			//++it;
			it += 3;
		}

		// team2, blue
		glm::vec3 color2 CHESS_COLOR2;
		shader.setVec3("objectColor", color2);
		it = chess_list.begin();
		++it;
		while (it<chess_list.end())
		{
			if ((*it)->check_death())
			{
				it += 3;
				continue;
			}
			glm::mat4 model;
			(*it)->get_model(model);
			shader.setMat4("model", model);
			(*it)->show(shader);
			//++it;
			it += 3;
		}

		// team3, red
		glm::vec3 color3 CHESS_COLOR3;
		shader.setVec3("objectColor", color3);
		it = chess_list.begin();
		it += 2;
		while (it<chess_list.end())
		{
			if ((*it)->check_death())
			{
				it += 3;
				continue;
			}
			glm::mat4 model;
			(*it)->get_model(model);
			shader.setMat4("model", model);
			(*it)->show(shader);
			//++it;
			it += 3;
		}
		return;
	}
	if (!if_explode)
	{
		vector<Chess *>::iterator it = chess_list.begin();
		while (it<chess_list.end())
		{
			if ((*it)->if_selected_ || (*it)->_if_explode || (*it)->check_death()) {
				it += 3;
				continue;
			}
			if (if_motion_blur)
			{
				shader.setInt("num_samples", 10);
				shader.setVec2("velocity", glm::vec2(0.1f, 0.1f));
			}
			else
			{
				shader.setInt("num_samples", 1);
				shader.setVec2("velocity", glm::vec2(0.0f, 0.0f));
			}
			glm::vec3 color1 CHESS_COLOR1;
			shader.setVec3("objectColor", color1);
			glm::mat4 model;
			(*it)->get_model(model);
			shader.setMat4("model", model);

			(*it)->show(shader);
			//++it;
			it += 3;
		}

		// team2, green
		glm::vec3 color2 CHESS_COLOR2;
		shader.setVec3("objectColor", color2);
		it = chess_list.begin();
		++it;
		while (it<chess_list.end())
		{
			if ((*it)->if_selected_ || (*it)->_if_explode || (*it)->check_death()) {
				it += 3;
				continue;
			}
			if (if_motion_blur)
			{
				shader.setInt("num_samples", 10);
				shader.setVec2("velocity", glm::vec2(0.1f, 0.1f));
			}
			else
			{
				shader.setInt("num_samples", 1);
				shader.setVec2("velocity", glm::vec2(0.0f, 0.0f));
			}
			glm::mat4 model;
			(*it)->get_model(model);
			shader.setMat4("model", model);

			(*it)->show(shader);
			//++it;
			it += 3;
		}

		// team3, red
		glm::vec3 color3 CHESS_COLOR3;
		shader.setVec3("objectColor", color3);
		it = chess_list.begin();
		it += 2;
		while (it<chess_list.end())
		{
			if ((*it)->if_selected_ || (*it)->_if_explode || (*it)->check_death()) {
				it += 3;
				continue;
			}
			if (if_motion_blur)
			{
				shader.setInt("num_samples", 10);
				shader.setVec2("velocity", glm::vec2(0.1f, 0.1f));
			}
			else
			{
				shader.setInt("num_samples", 1);
				shader.setVec2("velocity", glm::vec2(0.0f, 0.0f));
			}
			glm::mat4 model;
			(*it)->get_model(model);
			shader.setMat4("model", model);

			(*it)->show(shader);
			//++it;
			it += 3;
		}
		return;
	}
	else
	{
		vector<Chess *>::iterator it = chess_list.begin();
		while (it<chess_list.end())
		{
			if (!(*it)->_if_explode || (*it)->check_death()) {
				it += 3;
				continue;
			}
			glm::vec3 color1 CHESS_COLOR1;
			shader.setVec3("objectColor", color1);

			counter *= 1.4;
			glm::mat4 model;
			(*it)->get_model(model);
			shader.setMat4("model", model);
			if (counter >= 10.0f) {
				counter = 0.01;
				(*it)->_if_explode = false;
				(*it)->set_death();
			}
			shader.setFloat("time", counter);
			(*it)->show(shader);
			//++it;
			it += 3;
		}

		// team2, green
		glm::vec3 color2 CHESS_COLOR2;
		shader.setVec3("objectColor", color2);
		it = chess_list.begin();
		++it;
		while (it<chess_list.end())
		{
			if (!(*it)->_if_explode || (*it)->check_death()) {
				it += 3;
				continue;
			}
			counter *= 1.4;
			glm::mat4 model;
			(*it)->get_model(model);
			shader.setMat4("model", model);
			if (counter >= 20.0f) {
				counter = 0.01;
				(*it)->_if_explode = false;
				(*it)->set_death();
			}
			shader.setFloat("time", counter);
			(*it)->show(shader);
			//++it;
			it += 3;
		}

		// team3, red
		glm::vec3 color3 CHESS_COLOR3;
		shader.setVec3("objectColor", color3);
		it = chess_list.begin();
		it += 2;
		while (it<chess_list.end())
		{
			if (!(*it)->_if_explode || (*it)->check_death()) {
				it += 3;
				continue;
			}
			counter *= 1.4;
			glm::mat4 model;
			(*it)->get_model(model);
			shader.setMat4("model", model);
			if (counter >= 30.0f) {
				counter = 0.01;
				(*it)->_if_explode = false;
				(*it)->set_death();
			}
			shader.setFloat("time", counter);
			(*it)->show(shader);
			//++it;
			it += 3;
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