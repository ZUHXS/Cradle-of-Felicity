#pragma once
#include "chess.h"

class ChessViper : public Chess
{
public:
	ChessViper(int init_cor_x, int init_cor_y, int team, int id, std::string const &path, bool gamma = false);
	~ChessViper();
	virtual void show(Shader &input_shader);
};

