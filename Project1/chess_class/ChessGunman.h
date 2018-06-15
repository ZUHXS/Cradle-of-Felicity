#pragma once
#include "chess.h"

class ChessGunman : public Chess
{
public:
	ChessGunman(int init_cor_x, int init_cor_y, int team, int id, string const &path, bool gamma = false);
	~ChessGunman();
	virtual void show(Shader &input_shader);
};

