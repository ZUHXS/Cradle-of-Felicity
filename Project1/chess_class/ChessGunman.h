#pragma once
#include "chess.h"
#include "../include/camera.h"

class ChessGunman : public Chess
{
public:
	ChessGunman(int init_cor_x, int init_cor_y, int team, int id, string const &path, bool gamma = false);
	~ChessGunman();
	virtual void show(Shader &input_shader);
	void get_model(glm::mat4& model) override;
	void get_moving_function(int next_pos_x, int next_pos_y, const Camera &camera) override;
};

