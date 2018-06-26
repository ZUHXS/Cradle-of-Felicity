#include "../include/camera.h"
#include <GLFW/glfw3.h>

extern bool if_mouse_key_callback;
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
	Front(glm::vec3(-position.x, -position.y, -position.z)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY),
	Zoom(ZOOM)
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	If_Move_Auto = false;
	move_origin_position = glm::vec3(1.9f, 1.3f, 2.3f);
	move_destination = glm::vec3(-1.6f, 0.6f, 1.3f);
	destination_sight = move_destination - move_origin_position;
	move_duration = 1000;
	if_parabola = false;
	step = 0;
	updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
	Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;
	If_Move_Auto = false;
	move_origin_position = glm::vec3(1.9f, 1.3f, 2.3f);
	move_destination = glm::vec3(-1.6f, 0.6f, 1.3f);
	destination_sight = move_destination - move_origin_position;
	move_duration = 1000;
	if_parabola = false;
	step = 0;
	updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix(std::vector<Chess*>chess_list)
{
	//return glm::lookAt(Position, Position + Front, Up);
	static int x = 0;
	if(!If_Move_Auto)
		return glm::lookAt(Position, glm::vec3(0.0f, 0.0f, 0.0f), Up);
	else if (!if_parabola)
	{
		x += 1;
		if (x == move_duration)
		{
			this->If_Move_Auto = false;
			x = 0;
			if_mouse_key_callback = true;
		}
		
		glm::vec3 gun_position(move_origin_position + ((move_destination - move_origin_position) / static_cast<float>(move_duration)) * static_cast<float>(x));
		//glm::vec3 gun_front(move_destination - move_origin_position);
		return glm::lookAt(gun_position, destination_sight, Up);
	}
	else
	{
		x += 1;
		if (x == move_duration)
		{
			this->If_Move_Auto = false;
			x = 0;
			if_mouse_key_callback = true;
		}
		if (x == move_duration - 10)
		{
			for (int i = 0; i < chess_list.size(); i++)
			{
				if (chess_list[i]->check_death())   // if die, then continue
					continue;
				if (vertice_coordinate[chess_list[i]->get_cor_x()+9][chess_list[i]->get_cor_y() + 5][0] == move_destination.x && vertice_coordinate[chess_list[i]->get_cor_x() + 9][chess_list[i]->get_cor_y() + 5][2] == move_destination.z)
				{
					chess_list[i]->_if_explode = true;
					break;
				}
			}
		}
		const float gun_pos_x = move_origin_position.x + ((move_destination.x - move_origin_position.x) / static_cast<float>(move_duration)) * static_cast<float>(x);
		const float gun_pos_z = move_origin_position.z + ((move_destination.z - move_origin_position.z) / static_cast<float>(move_duration)) * static_cast<float>(x);
		const float gun_pos_y = move_origin_position.y + init_gun_vert_speed * x - 0.5 * G * pow(x, 2);

		const glm::vec3 gun_position(gun_pos_x, gun_pos_y, gun_pos_z);
		/*const auto x1 = move_destination.x;
		const auto x2 = move_origin_position.x;
		const auto y1 = move_destination.z;
		const auto y2 = move_origin_position.z;
		const glm::vec3 gun_position(gun_pos_x, gun_pos_y, gun_pos_z);
		const float des_vector_x = init_gun_hor_speed / (sqrt(((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2)) / ((x1 - x2)*(x1 - x2))));
		const float des_vector_y = init_gun_hor_speed / (sqrt(((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2)) / ((y1 - y2)*(y1 - y2))));
		const glm::vec3 destination_vector(des_vector_x, init_gun_vert_speed - G*x, des_vector_y);*/

		return glm::lookAt(gun_position, move_destination, Up);
	}
}


void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	static float x;
	static float redius = 3.0f;
	float velocity = MovementSpeed * deltaTime / 2;
	if (direction == FORWARD) {
		if (redius - velocity <= 0)
			return;
		Position.y = ((redius - velocity) / redius) * (float)Position.y;
		redius -= velocity;
		Position.x = sin(x) * redius;
		Position.z = cos(x) * redius;
	}
	//Position += Front * velocity;
	if (direction == BACKWARD)
	{
		Position.y = ((redius + velocity) / redius) * (float)Position.y;
		redius += velocity;
		Position.x = sin(x) * redius;
		Position.z = cos(x) * redius;
	}
	//Position -= Front * velocity;
	/*if (direction == LEFT)
	Position -= Right * velocity;
	if (direction == RIGHT)
	Position += Right * velocity;*/
	if (direction == LEFT)
	{
		x += velocity;
		Position.x = sin(x) * redius;
		Position.z = cos(x) * redius;
	}
	if (direction == RIGHT)
	{
		x -= velocity;
		Position.x = sin(x) * redius;
		Position.z = cos(x) * redius;
	}

}


void Camera::ProcessMouseMovement(float xoffset, float yoffset, std::vector<Chess *> chess_list, std::vector<ChessBoard *> Block_list, bool& mouse_button, GLboolean constrainPitch)
{
	static auto last_i = 0;
	static auto last_j = 0;
	static auto last_chess_index = -1;
	static auto last_block_index = 0;
	static auto first_step_i = 0;
	static auto first_step_j = 0;
	static auto first_block_index = 0;
	static auto first_chess_index = -1;
	static auto second_last_i = 0;
	static auto second_last_j = 0;
	static auto second_last_chess_index = -1;
	static auto second_last_block_index = 0;


	//glm::vec3 a(-4.76, -1.0, 0.136);
	//glm::vec3 aPos(-4.0f, -0.722f, -0.0f);
	if (!step)  // first step
	{
		const glm::vec3 a_pos(0.0f, 0.0f, 0.0f);


		const double normal_x = (static_cast<float>(xoffset) - 800.0f) / 800.0f;
		const double normal_y = (600.0f - static_cast<float>(yoffset)) / 600.0f;
		int min_i = 0;
		int min_j = 0;
		double min = 0.009;
		for (int i = -9; i <= 9; i++)
		{
			for (int j = chess_board_vertices[i + 9][0]; j <= chess_board_vertices[i + 9][1]; j++)
			{
				if ((i == -1 && j == 0) || (i == 1 && j == 0) || (i == 0 && j == -1))
					continue;
				const glm::vec3 a = glm::vec3(vertice_coordinate[i + 9][j + 5][0], vertice_coordinate[i + 9][j + 5][1], vertice_coordinate[i + 9][j + 5][2]);

				const auto projection = glm::perspective(glm::radians(Zoom), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 100.0f);
				const auto view = GetViewMatrix(chess_list);
				glm::mat4 model;
				model = glm::translate(model, a);
				model = glm::rotate(model, -3.14f / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));

				const glm::vec4 gl_position = projection * view * model * glm::vec4(a_pos, 1.0);
				//glm::vec4 gl_position = glm::vec4(a, 1.0) * model * view * projection;
				const double this_normal_x = gl_position.x / gl_position.w;
				const double this_normal_y = gl_position.y / gl_position.w;
				//printf("%f, %f, ", gl_position.x / gl_position.w, gl_position.y / gl_position.w);

				//printf("%f, %f\n", (static_cast<float>(xoffset) - 400.0f) / 400.0f, (300.0f - static_cast<float>(yoffset)) / 300.0f);
				const double cur_result = pow((this_normal_x - normal_x), 2) + pow((this_normal_y - normal_y), 2);
				if (min > cur_result)
				{
					min_i = i;
					min_j = j;
					float min_this_normal_x = this_normal_x;
					float min_this_normal_y = this_normal_y;
					min = cur_result;
				}

			}
		}

		if (min != 0.009) {
			int i;

			// first change the chess's select status
			for (i = 0; i < static_cast<int>(chess_list.size()); i++)
			{
				if (chess_list[i]->get_cor_x() == min_i && chess_list[i]->get_cor_y() == min_j && !chess_list[i]->check_death()) {
					chess_list[i]->if_selected_ = true;
					chess_list[i]->select_effect = 1;
					first_chess_index = i;
					break;
				}
			}

			// update the board's select status
			auto block_index = block_coor[min_i + 9][min_j + 5];
			if (block_index <= 60 && block_index != 0)
			{
				block_index -= 1;
				Block_list[block_index]->block_color = HIGHLIGHTCOLOR;
				first_block_index = block_index;
			}
			else if (block_index != 0)
			{
				block_index -= 2;
				Block_list[block_index]->block_color = HIGHLIGHTCOLOR;
				first_block_index = block_index;
			}


			// reset last chess's select status and block's status
			if ((min_i != last_i || min_j != last_j) && i != static_cast<int>(chess_list.size()))  // something on the board
			{
				chess_list[last_chess_index == -1 ? 0 : last_chess_index]->if_selected_ = false;
				chess_list[last_chess_index == -1 ? 0 : last_chess_index]->select_effect = 0;
				last_chess_index = i;
			}
			if ((min_i != last_i || min_j != last_j) && i == static_cast<int>(chess_list.size()))   // nothing on the board
			{
				chess_list[last_chess_index == -1 ? 0 : last_chess_index]->if_selected_ = false;
				chess_list[last_chess_index == -1 ? 0 : last_chess_index]->select_effect = 0;
				last_chess_index = -1;
			}
			last_i = min_i;
			last_j = min_j;

			if (block_index != last_block_index)
			{
				Block_list[last_block_index]->reset_color();
				last_block_index = block_index;
			}
		}
	}
	
	// second step
	else
	{
		const glm::vec3 a_pos(0.0f, 0.0f, 0.0f);

		const double normal_x = (static_cast<float>(xoffset) - 800.0f) / 800.0f;
		const double normal_y = (600.0f - static_cast<float>(yoffset)) / 600.0f;
		int min_i = 0;
		int min_j = 0;
		double min = 0.009;
		for (int i = -9; i <= 9; i++)
		{
			for (int j = chess_board_vertices[i + 9][0]; j <= chess_board_vertices[i + 9][1]; j++)
			{
				if ((i == -1 && j == 0) || (i == 1 && j == 0) || (i == 0 && j == -1))
					continue;
				const glm::vec3 a = glm::vec3(vertice_coordinate[i + 9][j + 5][0], vertice_coordinate[i + 9][j + 5][1], vertice_coordinate[i + 9][j + 5][2]);

				const auto projection = glm::perspective(glm::radians(Zoom), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 100.0f);
				const auto view = GetViewMatrix(chess_list);
				glm::mat4 model;
				model = glm::translate(model, a);
				model = glm::rotate(model, -3.14f / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));

				const glm::vec4 gl_position = projection * view * model * glm::vec4(a_pos, 1.0);
				const double this_normal_x = gl_position.x / gl_position.w;
				const double this_normal_y = gl_position.y / gl_position.w;
				
				const double cur_result = pow((this_normal_x - normal_x), 2) + pow((this_normal_y - normal_y), 2);
				if (min > cur_result)
				{
					min_i = i;
					min_j = j;
					float min_this_normal_x = this_normal_x;
					float min_this_normal_y = this_normal_y;
					min = cur_result;
				}

			}
		}

		if (min != 0.009) {
			if (second_last_i == first_step_i && second_last_j == first_step_j)
			{
				second_last_chess_index = 0;
				second_last_block_index = 0;
				second_last_i = 0;
				second_last_j = 0;
			}
			int i;

			// first change the chess's select status
			for (i = 0; i < static_cast<int>(chess_list.size()); i++)
			{
				if (chess_list[i]->get_cor_x() == min_i && chess_list[i]->get_cor_y() == min_j && !chess_list[i]->check_death() && i != first_chess_index) {
					chess_list[i]->if_selected_ = true;
					chess_list[i]->select_effect = 2;
					break;
				}
			}

			// update the board's select status
			auto block_index = block_coor[min_i + 9][min_j + 5];
			if (block_index <= 60 && block_index != 0)
			{
				if (block_index - 1 != first_block_index) {
					block_index -= 1;
					Block_list[block_index]->block_color = HIGHLIGHTCOLOR2;
				}
			}
			else if (block_index != 0)
			{
				if (block_index - 2 != first_block_index) {
					block_index -= 2;
					Block_list[block_index]->block_color = HIGHLIGHTCOLOR2;
				}
			}


			// reset last chess's select status and block's status
				
			if ((min_i != second_last_i || min_j != second_last_j) && i != static_cast<int>(chess_list.size()))
			{
				chess_list[second_last_chess_index == -1 ? 0 : second_last_chess_index]->if_selected_ = false;
				chess_list[second_last_chess_index == -1 ? 0 : second_last_chess_index]->select_effect = 0;
				second_last_chess_index = i;
				second_last_i = min_i;
				second_last_j = min_j;
			}
			if ((min_i != second_last_i || min_j != second_last_j) && i == static_cast<int>(chess_list.size()))
			{
				chess_list[second_last_chess_index == -1 ? 0 : second_last_chess_index]->if_selected_ = false;
				chess_list[second_last_chess_index == -1 ? 0 : second_last_chess_index]->select_effect = 0;
				second_last_chess_index = -1;
				second_last_i = min_i;
				second_last_j = min_j;
			}

			if (block_index != second_last_block_index)
			{
				Block_list[second_last_block_index]->reset_color();
				second_last_block_index = block_index;
			}
				
			
		}
	}


	if(mouse_button)  // if pressed
	{
		//printf("Press detected, cur step: %d\n", step);
		if (!step) {
			printf("selected %d, %d\n", last_i, last_j);
			if(last_chess_index == -1)
			{
				goto finish;   // no chess selected
			}
			step += 1;
			chess_list[first_chess_index]->if_selected_ = true;
			chess_list[first_chess_index]->select_effect = 1;
			Block_list[first_block_index]->block_color = HIGHLIGHTCOLOR;
			first_step_i = last_i;
			first_step_j = last_j;
		}
		else
		{
			//printf("second step.\n");
			step = 0;
			if (second_last_chess_index != -1 && !chess_list[first_chess_index]->id_if_gunman())
				chess_list[second_last_chess_index]->_if_explode = true;
			chess_list[last_chess_index]->get_moving_function(second_last_i, second_last_j, *this);
			chess_list[first_chess_index]->select_effect = 0;
			chess_list[first_chess_index]->if_selected_ = false;
			Block_list[first_block_index]->reset_color();
			Block_list[second_last_block_index]->reset_color();
			last_i = 0;
			last_j = 0;
			last_chess_index = -1;
			last_block_index = 0;
			first_step_i = 0;
			first_step_j = 0;
			second_last_i = 0;
			second_last_j = 0;
			second_last_chess_index = -1;
			second_last_block_index = 0;
			first_block_index = 0;
			first_chess_index = -1;
			
		}
	}
finish:
	mouse_button = false;
	updateCameraVectors();
}



void Camera::ProcessMouseScroll(float yoffset)
{
	if (Zoom >= 1.0f && Zoom <= 45.0f)
		Zoom -= yoffset;
	if (Zoom <= 1.0f)
		Zoom = 1.0f;
	if (Zoom >= 45.0f)
		Zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}