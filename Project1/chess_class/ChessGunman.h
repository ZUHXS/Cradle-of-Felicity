#pragma once
#include "chess.h"
#include "../include/camera.h"

class ChessGunman : public Chess
{
public:
	ChessGunman(int init_cor_x, int init_cor_y, int team, int id, string const &path, Camera &camera, bool gamma = false);
	~ChessGunman();
	virtual void show(Shader &input_shader);
	void get_model(glm::mat4& model) override;
	void get_moving_function(int next_pos_x, int next_pos_y, Camera &camera) override;
	int id_if_gunman() const override;
private:
	int next_c_cor_x;
	int next_c_cor_y;
	void normalize_angle();
	Camera *op_camera;
	int rotate_duration;
	int cur_status;  // for 0 to show normal, for 1 to rotate, for 2 to change the camera's position
	float angle;
};

