#pragma once
#include "chess.h"

class ChessQueen : public Chess
{
public:
	ChessQueen(int init_cor_x, int init_cor_y, int team, int id, std::string const &path, bool gamma = false);
	~ChessQueen();
	virtual void show(Shader &input_shader);
};

