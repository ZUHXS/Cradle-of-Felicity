#include "chess.h"

void Chess::get_model(glm::mat4 & model)
{
	
	model = glm::translate(model, pos_);
	model = glm::rotate(model, rotation_.x, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, rotation_.y, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, rotation_.z, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, scale_);
}

bool Chess::check_live_status() const
{
	return if_die_;
}

Chess::Chess(const int init_cor_x, const int init_cor_y, int id, int init_team, string const &path, const bool gamma) : player_(init_team), chess_model_(path, gamma), chess_id_(id)
{
	cor_x_ = init_cor_x;
	cor_y_ = init_cor_y;
	if_die_ = false;
	_if_explode = false;
	//pos_ = glm::vec3(0.0, 0.0, 0.0);
	pos_ = glm::vec3(static_cast<float>(init_cor_x)/10.0f, 0.0f, static_cast<float>(init_cor_y)/10.0f);
	rotation_ = glm::vec3(-3.14f/2, 0.0f, 0.0);
	//scale_ = glm::vec3(0.1, 0.1, 0.1);
	//scale_ = glm::vec3(0.005, 0.005, 0.005);
	scale_ = glm::vec3(0.1f, 0.1f, 0.1f);
}
