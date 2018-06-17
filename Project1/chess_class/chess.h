#pragma once
//#include "main.h"
#include "../model.h"

class Chess
{
public:
	Chess(const int init_cor_x, const int init_cor_y, int id, int init_team, string const &path, const bool gamma = false);
	virtual ~Chess() = default;
	virtual void show(Shader &input_shader) = 0;
	virtual void get_model(glm::mat4& model);
	bool check_live_status() const;
	Model chess_model_;
	int _if_explode;


private:
	glm::vec3 pos_;
	glm::vec3 rotation_;
	glm::vec3 scale_;

	int chess_id_;
	bool if_die_;  // need to be initialize
	int cor_x_;
	int cor_y_;
	int player_;
};

