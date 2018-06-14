#include "chess_bishop.h"



chess_bishop::chess_bishop(int init_cor_x, int init_cor_y, int team, int id,string const &path,  bool gamma) : Chess(init_cor_x, init_cor_y, id, team, path, gamma)
{

}


chess_bishop::~chess_bishop()
{
}

void chess_bishop::show(Shader &input_shader)
{
	if (check_live_status())
	{
		return;
	}
	this->chess_model_.Draw(input_shader);

}