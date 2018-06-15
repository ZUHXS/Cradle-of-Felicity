#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>


#include <iostream>
//#include "show_chess.hpp"
#include "chess.h"
#include "chess_Bishop.h"
#include "ChessQueen.h"
#include "ChessKing.h"
#include "ChessRook.h"
#include "ChessKnight.h"
#include "ChessViper.h"
#include "ChessWizard.h"
using namespace std;
#define MAX_CHESS_NUMBER 60


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void show_chess(std::vector<Chess *> &chess_list);