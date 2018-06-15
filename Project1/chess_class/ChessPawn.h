#pragma once
#include "chess.h"

class ChessPawn : public Chess
{
public:
	ChessPawn(int init_cor_x, int init_cor_y, int team, int id, string const &path, bool gamma = false);
	~ChessPawn();
	virtual void show(Shader &input_shader);
};

