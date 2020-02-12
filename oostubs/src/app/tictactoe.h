#ifndef OOSTUBS_APP_TICTACTOE_H
#define OOSTUBS_APP_TICTACTOE_H

#include "lib.h"

#include "user/thread.h"

namespace oostubs
{
	class TicTacToe : public Thread
	{
		protected:
			void execute( ) override;
	};
}

#endif

