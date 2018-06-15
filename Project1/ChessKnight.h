#pragma once
#include "chess.h"

class ChessKnight : public Chess
{
public:
	ChessKnight(int init_cor_x, int init_cor_y, int team, int id, std::string const &path, bool gamma = false);
	~ChessKnight();
	virtual void show(Shader &input_shader);
};

