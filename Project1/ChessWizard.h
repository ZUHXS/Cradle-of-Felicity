#pragma once
#include "chess.h"

class ChessWizard : public Chess
{
public:
	ChessWizard(int init_cor_x, int init_cor_y, int team, int id, std::string const &path, bool gamma = false);
	~ChessWizard();
	virtual void show(Shader &input_shader);
};

