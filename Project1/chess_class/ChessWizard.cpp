﻿#include "ChessWizard.h"


ChessWizard::ChessWizard(int init_cor_x, int init_cor_y, int team, int id, string const &path, bool gamma) : Chess(init_cor_x, init_cor_y, id, team, path, gamma)
{

}


ChessWizard::~ChessWizard()
{
}

void ChessWizard::show(Shader &input_shader)
{
	if (check_death())
	{
		return;
	}
	this->chess_model_.Draw(input_shader);

}