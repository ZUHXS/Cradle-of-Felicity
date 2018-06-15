#pragma once
#include "chess.h"

class ChessBishop : public Chess
{
public:
	ChessBishop(int init_cor_x, int init_cor_y, int team, int id, std::string const &path, bool gamma = false);
	~ChessBishop();
	virtual void show(Shader &input_shader);
};

