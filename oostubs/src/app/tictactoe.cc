#include "app/tictactoe.h"

#include "initrd.h"

#include "user/mouse.h"
#include "user/keyboard.h"

#include "gui/texture.h"
#include "gui/screen.h"
#include "gui/dif.h"

#include "lib/memory.h"

#define BOARD_X 180
#define BOARD_Y 180
#define BOARD_W 150
#define BOARD_H 120

namespace oostubs {

namespace
{
	struct Game
	{
		Game(Texture *field, Texture *p1, Texture *p2);

		int& at(uint x, uint y) { return board[x + y * 3]; }
		const int& at(uint x, uint y) const { return board[x + y * 3]; }
		bool done( ) const { return winner() || !left; }
		bool play(uint x, uint y);
		int winner( ) const;
		void reset( );

		int board[3 * 3];
		int player;
		uint left;
		Texture *field, *p1, *p2;
	};

	Game::Game(Texture *field, Texture *p1, Texture *p2)
		: field(field)
		, p1(p1)
		, p2(p2)
	{
		reset();
	}

	bool Game::play(uint x, uint y)
	{
		ASSERT(left);

		if(x < BOARD_X || y < BOARD_Y || x >= BOARD_X + 3 * BOARD_W || y >= BOARD_Y + 3 * BOARD_H)
			return false;

		x = (x - BOARD_X) / BOARD_W;
		y = (y - BOARD_Y) / BOARD_H;

		ASSERT(x < 3 && y < 3);

		if(at(x, y))
			return false;

		auto *cp = (player == 1 ? p1 : p2);

		field->blend_blt(*cp,
			BOARD_X + x * BOARD_W + (BOARD_W - cp->width()) / 2,
			BOARD_Y + y * BOARD_H + (BOARD_H - cp->height()) / 2);

		at(x, y) = player;
		player *= -1;
		--left;

		return done();
	}

	int Game::winner(void) const
	{
		for(int i = 0 ; i < 3 ; ++i)
		{
			int row = at(i, i);
			int col = at(i, i);

			for(int j = 0 ; j < 3 ; ++j)
			{
				if(i == j) continue;

				if(!row && !col) break;

				if(row != at(j, i)) row = 0;
				if(col != at(i, j)) col = 0;
			}

			if(row) return row;
			if(col) return col;
		}

		int c = at(1, 1);

		if(!c) return 0;

		if(c == at(0, 0) && c == at(2, 2))
			return c;

		if(c == at(0, 2) && c == at(2, 0))
			return c;

		return 0;
	}

	void Game::reset(void)
	{
		player = 1;
		memset(board, 0, sizeof(board));
		left = 9;
	}
}

void TicTacToe::execute(void)
{
	Texture cursor{dif_load_from_file(initrd_root->find("textures/diablo_cursor.dif"))};
	Texture bg{dif_load_from_file(initrd_root->find("textures/bg.dif"))};
	Texture p1{dif_load_from_file(initrd_root->find("textures/player_1.dif"))};
	Texture p2{dif_load_from_file(initrd_root->find("textures/player_2.dif"))};

	Texture field(bg.width(), bg.height());

	Screen& screen{FramebufferManager::instance()};
	Mouse& mouse{MouseManager::instance()};

	Game game(&field, &p1, &p2);

	field.blt(bg, 0, 0);

	while(true)
	{
		while(mouse.size() > 0)
		{
			auto e{mouse.get()};

			if(e.button == Mouse::Button::PRIMARY && e.pressed)
			{
				if(game.play(e.x, e.y))
				{
					field.blt(bg, 0, 0);
					game.reset();
				}
			}
		}

		screen.buffer().blt(field, 0, 0, false);
		screen.buffer().blt(cursor, mouse.position().x, mouse.position().y);

		screen.sync();
	}
}

}

