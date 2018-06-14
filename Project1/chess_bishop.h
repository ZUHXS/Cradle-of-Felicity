#pragma once
#include "chess.h"

class chess_bishop : public Chess
{
public:
	chess_bishop(int init_cor_x, int init_cor_y, int team, int id,string const &path,  bool gamma = false);
	~chess_bishop();
	virtual void show(Shader &input_shader);
};

