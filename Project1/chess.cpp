#include "chess.h"

const void Chess::get_model(glm::mat4 & model)
{
	model = glm::scale(model, scale_);
	model = glm::rotate(model, rotation_.x, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, rotation_.y, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, rotation_.z, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, pos_);
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
	pos_ = glm::vec3(1.0, 1.0, 0.0);
	rotation_ = glm::vec3(0.0, 0.0, 0.0);
	scale_ = glm::vec3(0.05, 0.05, 0.05);
}
