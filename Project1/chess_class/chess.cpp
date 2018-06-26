#include "chess.h"

void Chess::get_model(glm::mat4 & model)
{
	glm::vec3 process_position;
	static int counter = 0;
	if (if_moving)
	{
		if (counter == chess_move_duration)
		{
			if_moving = false;
			chess_move_duration = 0;
			counter = 0;
			pos_ = this->next_position;
			next_position = glm::vec3(0.0f, 0.0f, 0.0f);
			process_position = pos_;
		}
		else
		{
			const float chess_pos_x = pos_.x + (next_position.x - pos_.x) / static_cast<float>(chess_move_duration) * static_cast<float>(counter);
			const float chess_pos_z = pos_.z + (next_position.z - pos_.z) / static_cast<float>(chess_move_duration) * static_cast<float>(counter);
			const float chess_pos_y = pos_.y + chess_init_gun_vert_speed * counter - 0.5 * this->chess_G * pow(counter, 2);
			process_position = glm::vec3(chess_pos_x, chess_pos_y, chess_pos_z);
			counter += 1;
		}
	}
	else
	{
		process_position = pos_;
	}


	model = glm::translate(model, process_position);
	model = glm::rotate(model, rotation_.x, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, rotation_.y, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, rotation_.z, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, scale_);
}

void Chess::get_moving_function(int next_pos_x, int next_pos_y, const Camera &camera)
{
	cor_x_ = next_pos_x;
	cor_y_ = next_pos_y;
	this->next_position = glm::vec3(vertice_coordinate[next_pos_x + 9][next_pos_y + 5][0], vertice_coordinate[next_pos_x + 9][next_pos_y + 5][1], vertice_coordinate[next_pos_x + 9][next_pos_y + 5][2]);
	this->chess_move_duration = static_cast<int>(sqrt(((pow(pos_.x - this->next_position.x, 2) + pow(pos_.z - this->next_position.z, 2)))) / chess_init_gun_hor_speed);
	this->chess_init_gun_vert_speed = 0.5 * chess_G * this->chess_move_duration;
	//this->next_position = glm::vec3(vertice_coordinate[next_pos_x+9][next_pos_y+5][0], vertice_coordinate[next_pos_x+9][next_pos_y+5][1], vertice_coordinate[next_pos_x+9][next_pos_y+5][2]);
	//this->distance = 20;
	this->if_moving = true;
}

void Chess::set_death(bool status)
{
	if_die_ = status;
}


const int Chess::get_cor_x()
{
	return cor_x_;
}

const int Chess::get_cor_y()
{
	return cor_y_;
}

/**
 * \brief get live status
 * \return 1 if died
 */
bool Chess::check_death() const
{
	return if_die_;
}

Chess::Chess(const int init_cor_x, const int init_cor_y, int id, int init_team, string const &path, const bool gamma) : player_(init_team), chess_model_(path, gamma), chess_id_(id)
{
	cor_x_ = init_cor_x;
	cor_y_ = init_cor_y;
	if_die_ = false;
	_if_explode = false;
	if_selected_ = false;
	//pos_ = glm::vec3(0.0, 0.0, 0.0);
	pos_ = glm::vec3(vertice_coordinate[init_cor_x + 9][init_cor_y + 5][0], vertice_coordinate[init_cor_x + 9][init_cor_y + 5][1], vertice_coordinate[init_cor_x + 9][init_cor_y + 5][2]);
	rotation_ = glm::vec3(-3.14f/2, 0.0f, 0.0f);
	//scale_ = glm::vec3(0.1, 0.1, 0.1);
	//scale_ = glm::vec3(0.005, 0.005, 0.005);
	scale_ = glm::vec3(0.1f, 0.1f, 0.1f);
	if_moving = false;
	select_effect = 0;
	chess_init_gun_vert_speed = 0;
	chess_move_duration = 0;
}
