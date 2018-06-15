#pragma once
#include "chess.h"

class ChessRook : public Chess
{
public:
	ChessRook(int init_cor_x, int init_cor_y, int team, int id, std::string const &path, bool gamma = false);
	~ChessRook();
	virtual void show(Shader &input_shader);
};

