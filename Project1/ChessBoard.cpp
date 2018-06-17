#include "ChessBoard.h"



ChessBoard::ChessBoard(string const &path, glm::vec3 color, const bool gamma) : block_model(path, gamma)
{
	scale_ = glm::vec3(0.1f, 0.1f, 0.1f);
	block_color = color;
}


ChessBoard::~ChessBoard()
{
}

void ChessBoard::show(Shader &input_shader)
{
	this->block_model.Draw(input_shader);
}

void ChessBoard::get_model(glm::mat4& model)
{
	model = glm::scale(model, scale_);
	model = glm::rotate(model, -3.14f / 2, glm::vec3(1.0f, 0.0f, 0.0f));
}