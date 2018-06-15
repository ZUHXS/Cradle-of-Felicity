#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
//#include <learnopengl/camera.h>
#include "include/camera.h"

#include <iostream>
//#include "show_chess.hpp"
#include "chess_class/chess.h"
#include "chess_class/chess_Bishop.h"
#include "chess_class/ChessQueen.h"
#include "chess_class/ChessKing.h"
#include "chess_class/ChessRook.h"
#include "chess_class/ChessKnight.h"
#include "chess_class/ChessViper.h"
#include "chess_class/ChessWizard.h"
#include "chess_class/ChessGunman.h"
#include "chess_class/ChessPawn.h"
using namespace std;
#define MAX_CHESS_NUMBER 60


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void show_chess(std::vector<Chess *> &chess_list, Shader &shader);
void show_chess_board(Model &chess_board_model, Shader &shader);
void process_scene(Shader &shader, Shader &DepthShader, Shader &DepthQuad, std::vector<Chess *> &chess_list, Model &chess_board_model, unsigned int &depthMapFBO, unsigned int &depthMap);