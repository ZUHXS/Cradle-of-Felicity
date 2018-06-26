#include "ChessGunman.h"


ChessGunman::ChessGunman(int init_cor_x, int init_cor_y, int team, int id, string const &path, bool gamma) : Chess(init_cor_x, init_cor_y, id, team, path, gamma)
{

}


ChessGunman::~ChessGunman()
{
}

void ChessGunman::show(Shader &input_shader)
{
	if (check_death())
	{
		return;
	}
	this->chess_model_.Draw(input_shader);

}

void ChessGunman::get_model(glm::mat4& model)
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

void ChessGunman::get_moving_function(int next_pos_x, int next_pos_y, const Camera& camera)
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