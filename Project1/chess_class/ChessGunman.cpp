#include "ChessGunman.h"

extern bool if_mouse_key_callback;
ChessGunman::ChessGunman(int init_cor_x, int init_cor_y, int team, int id, string const &path, Camera &camera, bool gamma) : Chess(init_cor_x, init_cor_y, id, team, path, gamma)
{
	op_camera = &camera;
	rotate_duration = 0;
	cur_status = 0;
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
	if (cur_status)
	{
		if (cur_status == 1)   // rotate the chess
		{
			if (angle >= 3.1415f)
				angle -= 6.283f;
			rotation_.y = angle * static_cast<float>(counter) / static_cast<float>(this->rotate_duration);
			counter += 1;
			if (counter == this->rotate_duration)
			{
				cur_status = 2;
				counter = 0;
			}
			process_position = pos_;
		}
		if (2 == cur_status)   // camera
		{
			op_camera->move_origin_position = glm::vec3(vertice_coordinate[cor_x_ + 9][cor_y_ + 5][0], 0.5, vertice_coordinate[cor_x_ + 9][cor_y_ + 5][2]);
			//camera.move_destination = glm::vec3(vertice_coordinate[8 + 9][-1 + 5][0], 0.5, vertice_coordinate[8 + 9][-1 + 5][2]);
			op_camera->move_destination = glm::vec3(vertice_coordinate[next_c_cor_x + 9][next_c_cor_y + 5][0], 0.5, vertice_coordinate[next_c_cor_x + 9][next_c_cor_y + 5][2]);

			op_camera->move_duration = static_cast<int>(sqrt(((pow(op_camera->move_origin_position.x - op_camera->move_destination.x, 2) + pow(op_camera->move_origin_position.z - op_camera->move_destination.z, 2)))) / init_gun_hor_speed);
			op_camera->init_gun_vert_speed = 0.5 * G * op_camera->move_duration;
			printf("\nthe camera duration is %d, and the vert speed is %f\n", op_camera->move_duration, op_camera->init_gun_vert_speed);
			printf("vert is %f", op_camera->init_gun_vert_speed);
			//printf("%f, %f, %f, %f, %f\n", camera.move_origin_position.x, camera.mo)
			printf("move duration is: %d\n", op_camera->move_duration);
			op_camera->if_parabola = true;
			op_camera->If_Move_Auto = true;
			cur_status = 0;
			if_mouse_key_callback = false;
			//camera.If_Move_Auto = true;
		}
	}
	else
	{
		process_position = pos_;
	}


	model = glm::translate(model, process_position);
	model = glm::rotate(model, rotation_.y, glm::vec3(0.0f, 1.0f, 0.0f));  // 需要最后绕y转
	model = glm::rotate(model, rotation_.x, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, rotation_.z, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, scale_);
}

void ChessGunman::get_moving_function(int next_pos_x, int next_pos_y, Camera& camera)
{
	//static int counter = 0;
	// first calculate the angle to rotate
	if (!cur_status) {
		op_camera = &camera;
		float cur_x = vertice_coordinate[cor_x_ + 9][cor_y_ + 5][0];
		float cur_z = vertice_coordinate[cor_x_ + 9][cor_y_ + 5][2];
		float des_x = vertice_coordinate[next_pos_x + 9][5 + next_pos_y][0];
		float des_z = vertice_coordinate[next_pos_x + 9][5 + next_pos_y][2];
		this->next_c_cor_x = next_pos_x;
		this->next_c_cor_y = next_pos_y;
		float x_change = cur_x - des_x;
		float z_change = cur_z - des_z;
		this->angle = atan2(x_change, z_change);
		this->angle = this->angle - 3.14;
		normalize_angle();
		//printf("cur_x: %f, cur_z: %f, des_x: %f, des_z: %f\n", cur_x, cur_z, des_x, des_z);
		//printf("x_change: %f, z_change: %f, final result: %f\n", x_change, z_change, angle);
		this->rotate_duration = ((static_cast<int>(angle * 1000) + 6283*5) % 6283) / 20;
		printf("the duration: %d\n", this->rotate_duration);
		cur_status = 1;
	}



	//float angle = 3.14f/2.0f;
	//rotation_.y = angle - 3.14f;

	/*cor_x_ = next_pos_x;
	cor_y_ = next_pos_y;
	this->next_position = glm::vec3(vertice_coordinate[next_pos_x + 9][next_pos_y + 5][0], vertice_coordinate[next_pos_x + 9][next_pos_y + 5][1], vertice_coordinate[next_pos_x + 9][next_pos_y + 5][2]);
	this->chess_move_duration = static_cast<int>(sqrt(((pow(pos_.x - this->next_position.x, 2) + pow(pos_.z - this->next_position.z, 2)))) / chess_init_gun_hor_speed);
	this->chess_init_gun_vert_speed = 0.5 * chess_G * this->chess_move_duration;
	this->if_moving = true;*/
}

int ChessGunman::id_if_gunman() const
{
	return 1;
}

void ChessGunman::normalize_angle()
{
	while(angle >= 6.283f)
	{
		angle -= 6.283f;
	}
	while(angle <= 0.0f)
	{
		angle += 6.283f;
	}
}
