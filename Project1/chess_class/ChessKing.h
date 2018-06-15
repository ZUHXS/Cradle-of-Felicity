#pragma once
#include "chess.h"
#include <string>

class ChessKing : public Chess
{
public:
	ChessKing(int init_cor_x, int init_cor_y, int team, int id, std::string const &path, bool gamma = false);
	~ChessKing();
	virtual void show(Shader &input_shader);
};

