#pragma once
#include "model.h"

class ChessBoard
{
public:
	ChessBoard(string const &path, glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f), const bool gamma = false);
	~ChessBoard();
	void show(Shader &input_shader);
	void get_model(glm::mat4& model);
	Model block_model;
	glm::vec3 block_color;
private:
	glm::vec3 scale_;
};

