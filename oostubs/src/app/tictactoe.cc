#include "app/tictactoe.h"

#include "initrd.h"

#include "user/mouse.h"
#include "user/keyboard.h"

#include "gui/texture.h"
#include "gui/screen.h"
#include "gui/dif.h"

#include "lib/memory.h"
#include "lib/rng.h"
#include "lib/vector.h"

#define BOARD_X 180
#define BOARD_Y 180
#define BOARD_W 150
#define BOARD_H 120

namespace oostubs { namespace {

inline u32 compute_seed(void)
{
	static u32 old = 0;

	const auto& p{MouseManager::instance().position()};

	uint base = p.x * p.y * Screen::WIDTH;
	rng_t rng(base ^ old);

	old ^= rng();
	old ^= rng();
	old ^= rng();

	return old;
}

struct State
{
	State(int p = 1)
	{
		ASSERT(p == 1 || p == -1);
		memset(board, 0, sizeof(board));
		player = p;
		left = 9;
	}

	int& at(uint x, uint y) { ASSERT(x < 3u && y < 3u); return board[x + y * 3]; }
	int at(uint x, uint y) const { ASSERT(x < 3u && y < 3u); return board[x + y * 3]; }

	int winner( ) const;

	int board[3 * 3];
	int player;
	uint left;
};

class AI
{
	public:
		virtual ~AI( ) { }

		virtual void select( ) = 0;
		virtual uint decide(const State&) = 0;
};

class RandomAI : public AI
{
	public:
		void select( ) override { mRNG.seed(compute_seed()); }
		uint decide(const State&) override;
	
	private:
		rng_t mRNG;
};

class Animation
{
	public:
		virtual ~Animation( ) { }

		virtual void apply(Texture&) = 0;
		virtual bool done( ) const = 0;
		virtual void tick( ) = 0;
};

class TimedAnimation : public Animation
{
	public:
		TimedAnimation(uint t) : mLeft(t) { }

		bool done( ) const override { return !mLeft; }
		void tick( ) override { if(mLeft) --mLeft; }

	private:
		uint mLeft;
};

class DelayAnimation : public TimedAnimation
{
	public:
		DelayAnimation(uint t) : TimedAnimation(t) { }

		void apply(Texture&) override { }
};

class SlideAnimation : public TimedAnimation
{
	static constexpr i64 SCALE = 0x1000;

	public:
		SlideAnimation(const Texture&, int, int, int, int, uint);

		void apply(Texture&) override;
		void tick( ) override;

	private:
		const Texture *mTx;
		i64 mX, mY;
		i64 mDX, mDY;
};

class ProxyAnimation : public Animation
{
	public:
		void apply(Texture&) override { }
		bool done( ) const override { return true; }
		void tick( ) override { call(); }
	
	protected:
		virtual void call( ) = 0;
};

struct Game;

class ResetAnimation : public ProxyAnimation
{
	public:
		ResetAnimation(Game *g) : mSelf(g) { }

	protected:
		void call( ) override;
	
	private:
		Game *mSelf;
};

class WriteViewAnimation : public ProxyAnimation
{
	public:
		WriteViewAnimation(Texture&, const Texture&, uint, uint, bool = false);

		void apply(Texture&) override;

	protected:
		void call( ) override;

	private:
		Texture *mView;
		const Texture *mSrc;
		uint mX, mY;
		bool mWriteThrough;
};

class AddPieceAnimation : public WriteViewAnimation
{
	public:
		AddPieceAnimation(Game *, int, uint, uint);
};

class Game
{
	public:
		Game(Texture&);

		void update( );
		void click(uint, uint);

		void addAI(AI *ai) { mAIs.push_back(ai); }
	
	private:
		void eventPlay(int, uint, uint);
		void reset( );
		
	private:
		Texture *mScreen;
		State mState;
		AI *mAI;
		Texture mBackground;
		Texture mView;
		Texture mPlayer[2];
		Texture mTxWin, mTxLose;
		FixedRingbuffer<Animation *, 0x100> mAnimations;
		Vector<AI *> mAIs;

		friend class ResetAnimation;
		friend class AddPieceAnimation;
};

int State::winner(void) const
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

uint RandomAI::decide(const State& s)
{
	ASSERT(s.left);

	while(true)
	{
		uint i = mRNG() % 9;

		if(!s.board[i])
			return i;
	}
}

SlideAnimation::SlideAnimation(const Texture& tx, int x0, int y0, int x1, int y1, uint t)
	: TimedAnimation(t)
	, mTx(&tx)
{
	mDX = (x1 - x0) * SCALE / (int) t;
	mDY = (y1 - y0) * SCALE / (int) t;
	mX = x0 * SCALE;
	mY = y0 * SCALE;
}

void SlideAnimation::apply(Texture& target)
{
	target.blt(*mTx, mX / SCALE, mY / SCALE);
}

void SlideAnimation::tick(void)
{
	TimedAnimation::tick();

	mX += mDX;
	mY += mDY;
}

void ResetAnimation::call(void)
{
	mSelf->reset();
}

WriteViewAnimation::WriteViewAnimation(Texture& dst, const Texture& src, uint x, uint y, bool wt)
	: mView(&dst)
	, mSrc(&src)
	, mX(x)
	, mY(y)
	, mWriteThrough(wt)
{
}

void WriteViewAnimation::call(void)
{
	mView->blend_blt(*mSrc, mX, mY);
}

void WriteViewAnimation::apply(Texture& tx)
{
	if(mWriteThrough)
	{
		tx.blt(*mSrc, mX, mY);
	}
}

AddPieceAnimation::AddPieceAnimation(Game *self, int player, uint x, uint y)
	: WriteViewAnimation(
		self->mView, self->mPlayer[player == 1 ? 0 : 1],
		BOARD_X + x * BOARD_W + (BOARD_W - self->mPlayer[player == 1 ? 0 : 1].width()) / 2,
		BOARD_Y + y * BOARD_H + (BOARD_H - self->mPlayer[player == 1 ? 0 : 1].height()) / 2)
{
}

Game::Game(Texture& tx)
	: mScreen(&tx)
	, mAI(nullptr)
	, mBackground(dif_load_from_file(initrd_root->find("textures/bg.dif")))
	, mView(mBackground)
	, mPlayer({
		dif_load_from_file(initrd_root->find("textures/player_1.dif")),
		dif_load_from_file(initrd_root->find("textures/player_2.dif"))
	})
	, mTxWin(dif_load_from_file(initrd_root->find("textures/win.dif")))
	, mTxLose(dif_load_from_file(initrd_root->find("textures/lose.dif")))
{
}

void Game::update(void)
{
	mScreen->blt(mView, 0, 0);

	if(!mAI)
	{
		ASSERT(!mAIs.empty());

		rng_t rng(compute_seed());

		mAI = mAIs.at(rng() % mAIs.size());
		mAI->select();
	}

	if(!mAnimations.empty())
	{
		Animation *a = mAnimations.top();

		a->apply(*mScreen);
		a->tick();

		if(a->done())
		{
			mAnimations.pop();

			delete a;
		}
	}
	else
	{
		if(mState.player == -1)
		{
			uint i = mAI->decide(mState);

			eventPlay(-1, i % 3, i / 3);
		}
	}

	if(mAnimations.empty())
	{
		int w = mState.winner();
		Texture *tx = nullptr;
		uint y = 0;

		if(w)
		{
			tx = &(w == 1 ? mTxWin : mTxLose);
		}
		else if(!mState.left)
		{
			tx = &mTxLose;
		}

		if(tx)
		{
			y = (Screen::HEIGHT - tx->height()) / 2;

			mAnimations.push(new SlideAnimation(*tx, tx->width(), y, 0, y, 100));
			mAnimations.push(new WriteViewAnimation(mView, *tx, 0, y, true));
			mAnimations.push(new DelayAnimation(300));
			mAnimations.push(new ResetAnimation(this));
		}
	}
}

void Game::click(uint x, uint y)
{
	if(!mAnimations.empty())
		return;

	if(x < BOARD_X || y < BOARD_Y || x >= BOARD_X + 3 * BOARD_W || y >= BOARD_Y + 3 * BOARD_H)
		return;
	
	x = (x - BOARD_X) / BOARD_W;
	y = (y - BOARD_Y) / BOARD_H;

	if(mState.at(x, y))
		return;
	
	eventPlay(1, x, y);
}

void Game::eventPlay(int player, uint x, uint y)
{
	ASSERT(player == 1 || player == -1);
	ASSERT(x < 3 && y < 3);
	ASSERT(!mState.at(x, y));
	ASSERT(mState.left);

	mState.at(x, y) = player;
	mState.player = -1 * player;
	--mState.left;

	mAnimations.push(new AddPieceAnimation(this, player, x, y));
}

void Game::reset(void)
{
	mState = {};
	mAI = nullptr;
	mView.blt(mBackground, 0, 0);
}

}

void TicTacToe::execute(void)
{
	Screen& screen{FramebufferManager::instance()};
	Mouse& mouse{MouseManager::instance()};
	Texture cursor{dif_load_from_file(initrd_root->find("textures/diablo_cursor.dif"))};

	Game the_game(screen.buffer());

	the_game.addAI(new RandomAI);

	while(true)
	{
		the_game.update();

		while(mouse.size())
		{
			auto e{mouse.get()};

			if(e.button == Mouse::Button::PRIMARY && e.pressed)
			{
				the_game.click(e.x, e.y);
			}
		}

		screen.buffer().blt(cursor, mouse.position().x, mouse.position().y);

		screen.sync();
	}
}

}

