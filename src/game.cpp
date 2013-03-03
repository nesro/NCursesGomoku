/**
 * @file game.cpp
 * @brief Directs whole game.
 * 
 * @author Tomas Nesrovnal nesro@nesro.cz
 * @version 1.0
 */

#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <exception>

/******************************************************************************/

#include "game.hpp"
#include "board.hpp"
#include "player.hpp"
#include "ai.hpp"
#include "local.hpp"
#include "network.hpp"
#include "server.hpp"
#include "client.hpp"

/******************************************************************************/

CGame::CGame(int argc, char ** argv) {

	this->m_PlayerOnTurn = 0;
	this->m_ServerSocket = 0;
	this->m_ConnectionSocket = 0;

	if (argc > 1) {
		m_Port = atoi(argv[1]);
	} else {
		m_Port = CNetwork::DEFAULT_PORT;
	}

	if (argc > 2) {
		m_Host = argv[2];
	} else {
		m_Host = "localhost";
	}

	m_IsConnectionUp = false;
	m_IsServerUp = false;

	m_Players[0] = NULL;
	m_Players[1] = NULL;

	srand((unsigned) (time(NULL)));

	m_Status = STATUS_MENU;

	initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();

	/* turn off cursor */
	curs_set(0);

	start_color();
	init_pair(1, COLOR_CYAN, COLOR_BLACK);
	init_pair(6, COLOR_MAGENTA, COLOR_BLACK);

	/* color of old stones*/
	init_pair(2, COLOR_WHITE, COLOR_RED);
	init_pair(3, COLOR_WHITE, COLOR_GREEN);

	/* color of newest stone*/
	init_pair(4, COLOR_BLACK, COLOR_RED);
	init_pair(5, COLOR_BLACK, COLOR_GREEN);

	init_pair(BACKROUND_COLOR_X_PAIR, BACKROUND_COLOR_X, COLOR_BLACK);
	init_pair(BACKROUND_COLOR_O_PAIR, BACKROUND_COLOR_O, COLOR_BLACK);

	m_Board = new CBoard();
}

/******************************************************************************/

CGame::~CGame(void) {

	playerCleanup();

	delete m_Board;
	endwin();
}

/******************************************************************************/

void CGame::playerCleanup(void) {
	delete m_Players[0];
	m_Players[0] = NULL;

	delete m_Players[1];
	m_Players[1] = NULL;
}

/******************************************************************************/

void CGame::networkCleanup(void) {
	if (m_IsConnectionUp) {
		close(m_ConnectionSocket);
	}

	if (m_IsServerUp) {
		close(m_ServerSocket);
	}
}

/******************************************************************************/

void CGame::renderBoard(void) const {
	clearScreen();
	m_Board->show();
}

/******************************************************************************/

void CGame::startGame(int playerToStart) {
	m_Status = STATUS_TURN;

	m_PlayerOnTurn = playerToStart;

	clearScreen();
	m_Board->reset();
	m_Board->show();
}

/******************************************************************************/

void CGame::showMenu(void) {
	int key;
	int row = 0;
	int col = 0;

	clearScreen();

	mvprintw(row++, col, "/=====================================\\");
	mvprintw(row++, col, "|                                     |");
	mvprintw(row++, col, "| Program: \"NCursesGomoku\"            |");
	mvprintw(row++, col, "| Author:  \"nesrotom@fit.cvut.cz\"     |");
	mvprintw(row++, col, "|                                     |");
	mvprintw(row++, col, "|                                     |");
	mvprintw(row++, col, "| You can specify host and port with  |");
	mvprintw(row++, col, "| this usage:                         |");
	mvprintw(row++, col, "| nesrotom port host                  |");
	mvprintw(row++, col, "| if not, default is:                 |");
	mvprintw(row++, col, "| nesrotom %d localhost             |",
			CNetwork::DEFAULT_PORT);
	mvprintw(row++, col, "|                                     |");
	mvprintw(row++, col, "|                                     |");
	mvprintw(row++, col, "| \"1\" or \"a\" - New AI game            |");

	/* if we are clinet, we cannot convert to server */
	if ((m_IsServerUp == false && m_IsConnectionUp == false)
			|| (m_IsServerUp == true && m_IsConnectionUp == true)) {
		mvprintw(row++, col, "| \"2\" or \"s\" - New NET game (server)  |");
	}

	/* if we are server, we cannot convert to client */
	if ((m_IsServerUp == false && m_IsConnectionUp == false)
			|| (m_IsServerUp == false && m_IsConnectionUp == true)) {
		mvprintw(row++, col, "| \"3\" or \"c\" - New NET game (client)  |");
	}

	mvprintw(row++, col, "| \"4\" or \"w\" - Watch AI vs AI         |");
	mvprintw(row++, col, "| \"5\" or \"h\" - 2 players hotseat      |");
	mvprintw(row++, col, "| \"6\" or \"q\" - Quit                   |");
	mvprintw(row++, col, "|                                     |");
	mvprintw(row++, col, "\\=====================================/");

	move(0, 0);
	key = getch();

	/* just for safety */
	playerCleanup();

	switch (key) {
	case '1':
	case 'a':
	case 'A':
		m_Players[0] = new CLocal(m_Board, PLAYER_0_CHAR);
		m_Players[1] = new CAI(m_Board, PLAYER_1_CHAR);

		/* let random decide, who starts! */
		startGame(rand() % 2);
		break;
	case '2':
	case 's':
	case 'S':

		/* no for client */
		if (m_IsServerUp == false && m_IsConnectionUp == true) {
			showMenu();
			break;
		}

		m_Players[0] = new CLocal(m_Board, PLAYER_0_CHAR);
		m_Players[1] = new CServer(m_Board, PLAYER_1_CHAR, this);

		/* I (server) starts */
		startGame(0);
		break;
	case '3':
	case 'c':
	case 'C':

		/* no for server */
		if (m_IsServerUp) {
			showMenu();
			break;
		}

		m_Players[0] = new CLocal(m_Board, PLAYER_0_CHAR);
		m_Players[1] = new CClient(m_Board, PLAYER_1_CHAR, this);

		/* He (server) starts */
		startGame(1);
		break;
	case '4':
	case 'w':
	case 'W':
		m_Players[0] = new CAI(m_Board, PLAYER_0_CHAR);
		m_Players[1] = new CAI(m_Board, PLAYER_1_CHAR);

		/* random AI starts*/
		startGame(rand() % 2);

		break;
	case '5':
	case 'h':
	case 'H':
		m_Players[0] = new CLocal(m_Board, PLAYER_0_CHAR);
		m_Players[1] = new CLocal(m_Board, PLAYER_1_CHAR);

		startGame(0);
		break;
	case '6':
	case 'q':
	case 'Q':
		m_Status = CGame::STATUS_EXIT;
		break;
	default:
		showMenu();
		break;
	}
}

/******************************************************************************/

void CGame::playerMove(void) {
	m_Players[m_PlayerOnTurn]->nextMove();

	if (m_PlayerOnTurn == 0) {
		m_PlayerOnTurn = 1;
	} else {
		m_PlayerOnTurn = 0;
	}
}

/******************************************************************************/

void CGame::checkForWin(void) {
	int x;
	int y;
	int direction;
	char message[CBoard::MESSAGE_LENGTH];

	int playerToCheck = (m_PlayerOnTurn + 1) % 2;

	/* sequences of "xxxxx" and "ooooo" */
	char playerWinningSequence[2][6] = { { PLAYER_0_CHAR, PLAYER_0_CHAR,
			PLAYER_0_CHAR, PLAYER_0_CHAR, PLAYER_0_CHAR, '\0' }, {
			PLAYER_1_CHAR, PLAYER_1_CHAR, PLAYER_1_CHAR, PLAYER_1_CHAR,
			PLAYER_1_CHAR, '\0' } };

	if (m_Players[playerToCheck]->m_Surrender) {

		lastMove();

		memset(message, '\0', CBoard::MESSAGE_LENGTH);
		sprintf(message, " (%c) surrender! Press \"q\".",
				m_Players[playerToCheck]->m_Mark);

		m_Board->printMessage(message);
		usleep(1000000);

		playerCleanup();

		waitForChar('q', 'Q');

		m_Status = STATUS_MENU;
		return;
	}

	if (m_Board->m_StonesOnBoard
			>= m_Board->BOARD_HEIGHT * m_Board->BOARD_HEIGHT) {

		lastMove();

		m_Board->printMessage(" Draw! Press \"q\".");

		playerCleanup();

		waitForChar('q', 'Q');

		m_Status = STATUS_MENU;
		return;
	}

	/* find for winning sequence */
	if (m_Board->searchFor(x, y, direction,
			playerWinningSequence[playerToCheck])) {

		lastMove();

		memset(message, '\0', CBoard::MESSAGE_LENGTH);
		sprintf(message, " Player \"%c\" WINS! Press \"q\".",
				m_Players[playerToCheck]->m_Mark);

		m_Board->printMessage(message);

		waitForChar('q', 'Q');

		playerCleanup();

		m_Status = STATUS_MENU;
	}
}

/******************************************************************************/

void CGame::clearScreen(void) const {
	for (int i = 0; i < LINES; i++) {
		for (int j = 0; j < COLS; j++) {
			if (rand() % 2 == 0) {
				attron(COLOR_PAIR(BACKROUND_COLOR_X_PAIR));
				mvprintw(i, j, ".");
				attroff(COLOR_PAIR(BACKROUND_COLOR_X_PAIR));
			} else {
				attron(COLOR_PAIR(BACKROUND_COLOR_O_PAIR));
				mvprintw(i, j, "o");
				attroff(COLOR_PAIR(BACKROUND_COLOR_O_PAIR));
			}
		}
	}
}

/******************************************************************************/

void CGame::waitForChar(char waitForSmall, char waitForBig) const {
	int key;

	do {
		key = getch();
	} while (key != waitForSmall && key != waitForBig);
}

/******************************************************************************/

void CGame::lastMove() {

	/* beacuse server-client the last step founded winner set */
	m_Players[m_PlayerOnTurn]->m_IsWinner = true;

	/* send data about the surrender */
	m_Players[m_PlayerOnTurn]->nextMove();
}
