#include "chess.h"

void Chess::get_model(glm::mat4 & model)
{
	glm::vec3 process_position;
	static int counter = 0;
	if (if_moving)
	{
		counter += 1;
		process_position = (next_position - pos_) * static_cast<float>(counter) / static_cast<float>(distance) + pos_;
		if(counter == distance)
		{
			counter = 0;
			pos_ = next_position;
			next_position = glm::vec3(0.0f, 0.0f, 0.0f);
			if_moving = false;
			distance = 0;
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

void Chess::get_moving_function(int next_pos_x, int next_pos_y)
{
	this->next_position = glm::vec3(vertice_coordinate[next_pos_x+9][next_pos_y+5][0], vertice_coordinate[next_pos_x+9][next_pos_y+5][1], vertice_coordinate[next_pos_x+9][next_pos_y+5][2]);
	this->distance = 20;
	this->if_moving = true;
}

void Chess::set_death()
{
	if_die_ = true;
}

const bool Chess::check_death()
{
	return if_die_;
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
	if_selected_ = false;
	//pos_ = glm::vec3(0.0, 0.0, 0.0);
	pos_ = glm::vec3(vertice_coordinate[init_cor_x + 9][init_cor_y + 5][0], vertice_coordinate[init_cor_x + 9][init_cor_y + 5][1], vertice_coordinate[init_cor_x + 9][init_cor_y + 5][2]);
	rotation_ = glm::vec3(-3.14f/2, 0.0f, 0.0);
	//scale_ = glm::vec3(0.1, 0.1, 0.1);
	//scale_ = glm::vec3(0.005, 0.005, 0.005);
	scale_ = glm::vec3(0.1f, 0.1f, 0.1f);
	if_moving = false;
}
