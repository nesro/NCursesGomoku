/**
 * @file game.hpp
 * @brief Header file for class that directs whole game.
 * 
 * @author Tomas Nesrovnal nesro@nesro.cz
 * @version 1.0
 */

#include <iostream>
#include <ncurses.h>

/******************************************************************************/

#include "player.hpp"
#include "board.hpp"

/******************************************************************************/

#ifndef __GAME_HPP__

/**
 * For safety about multiple include
 */
#define	__GAME_HPP__

#define BACKROUND_COLOR_X COLOR_RED
#define BACKROUND_COLOR_O COLOR_MAGENTA
#define BACKROUND_COLOR_X_PAIR 12
#define BACKROUND_COLOR_O_PAIR 13

/**
 * @brief Main class directing game.
 */
class CGame {
public:

	/**
	 * Status of the game.
	 */
	int m_Status;

	/**
	 * Status for exit game.
	 */
	static const int STATUS_EXIT = 0;

	/**
	 * Status for show menu.
	 */
	static const int STATUS_MENU = 1;

	/**
	 * Status for waiting for player's move.
	 */
	static const int STATUS_TURN = 2;

	/**
	 * Old mark in game board for player 0.
	 */
	static const char PLAYER_0_CHAR = 'x';

	/**
	 * New mark in game board for player 0.
	 */
	static const char PLAYER_0_CHAR_BIG = 'X';

	/**
	 * Old mark in game board for player 1.
	 */
	static const char PLAYER_1_CHAR = 'o';

	/**
	 * New mark in game board for player 1.
	 */
	static const char PLAYER_1_CHAR_BIG = '0';

	/**
	 * Prepare objects for game.
	 * @param argc Program argument count
	 * @param argv Program arguments
	 */
	CGame(int argc, char ** argv);

	/**
	 * Free alocated objects.
	 */
	~CGame(void);

	/**
	 * Shows menu and wait for choose.
	 */
	void showMenu(void);

	/**
	 * Shows gameboard.
	 */
	void renderBoard(void) const;

	/**
	 * Reset variables and set status to turn.
	 * @param playerToStart who starts
	 */
	void startGame(int playerToStart);

	/**
	 * Waits for player move.
	 */
	void playerMove(void);

	/**
	 * Look for sequence of 5 stones on board or if all board is full of stones
	 * or if some player make himself surrender.
	 */
	void checkForWin(void);

	/**
	 * Clear all screen.
	 */
	void clearScreen(void) const;

	/**
	 * Free players objects.
	 */
	void playerCleanup(void);

	/**
	 * Close network connections
	 */
	void networkCleanup(void);

	/**
	 * Waits until key is pressed.
	 * @param waitForSmall Char to wait in lowercase
	 * @param waitForBig Char to wait in uppercase
	 */
	void waitForChar(char waitForSmall, char waitForBig) const;

	/**
	 * Provides last move, to send client-server data.
	 */
	void lastMove();

	/**
	 * Is server up?
	 */
	bool m_IsServerUp;

	/**
	 * Server socket.
	 */
	int m_ServerSocket;

	/**
	 * Connection socket.
	 */
	int m_ConnectionSocket;

	/**
	 * Is there connection?
	 */
	bool m_IsConnectionUp;

	/**
	 * Where on network is server?
	 */
	std::string m_Host;

	/**
	 * Port to connect
	 */
	int m_Port;

	/**
	 * Two containers for players.
	 */
	CPlayer *m_Players[2];

protected:

	/**
	 * Pointer to game board.
	 */
	CBoard *m_Board;

	/**
	 * Which player is on turn. (0 or 1)
	 */
	int m_PlayerOnTurn;
};

#endif	/* __GAME_HPP__ */
