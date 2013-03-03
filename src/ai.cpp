/**
 * @file ai.cpp
 * @brief Artificial intelligence opponent.
 * 
 * @author Tomas Nesrovnal nesro@nesro.cz
 * @version 1.0
 */

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>

/******************************************************************************/

#include "ai.hpp"
#include "player.hpp"

/******************************************************************************/

CAI::CAI(CBoard *board, char mark) :
		CPlayer(board, mark) {

	char opponentMark;

	if (mark == CGame::PLAYER_0_CHAR) {
		opponentMark = CGame::PLAYER_1_CHAR;
	} else {
		opponentMark = CGame::PLAYER_0_CHAR;
	}

	/*
	 * First is next move, if a strategy is found. A is AI and O is opponent.
	 * 0 is EMPTY_CELL
	 * 1 is AI
	 * 2 is opponent
	 * -1 is end of pattern
	 */
	int strategy[STRATEGIES][PATTERN_LENGTH] = {

	/* my four, go for win */
	{ 0, /* */0, 1, 1, 1, 1, -1, -1, -1, -1 }, //".AAAA"0
			{ 1, /* */1, 0, 1, 1, 1, -1, -1, -1, -1 }, //"A.AAA"1
			{ 2, /* */1, 1, 0, 1, 1, -1, -1, -1, -1 }, //"AA.AA"2

			/* his four, need to block */
			{ 0, /* */0, 2, 2, 2, 2, -1, -1, -1, -1 }, //".OOOO"0
			{ 1, /* */2, 0, 2, 2, 2, -1, -1, -1, -1 }, //"O.OOO"1
			{ 2, /* */2, 2, 0, 2, 2, -1, -1, -1, -1 }, //"OO.OO"2

			/* my three, go for four*/
			{ 0, /* */0, 1, 1, 1, 0, -1, -1, -1, -1 }, //".AAA."0
			{ 2, /* */0, 1, 0, 1, 1, -1, -1, -1, -1 }, //".A.AA"2
			{ 1, /* */1, 0, 1, 1, 0, -1, -1, -1, -1 }, //"A.AA."1
			{ 1, /* */0, 0, 1, 1, 1, -1, -1, -1, -1 }, //"..AAA"1

			/* his three, need to block*/
			{ 0, /* */0, 2, 2, 2, 0, -1, -1, -1, -1 }, //".OOO."0
			{ 1, /* */2, 0, 2, 2, 0, -1, -1, -1, -1 }, //"O.OO."1
			{ 1, /* */0, 2, 0, 2, 2, -1 - 1, -1, -1 }, //".O.OO"2

			/* my two, let's make three*/
			{ 2, /* */0, 1, 0, 1, 0, -1, -1, -1, -1 }, //".A.A."2
			{ 1, /* */0, 0, 1, 1, 0, -1, -1, -1, -1 }, //"..AA."1
			{ 1, /* */0, 0, 1, 1, 0, 0, -1, -1, -1 }, //"..AA.."1

			/* really nothing to do, lets make mark next to another */
			{ 1, /* */0, 0, 1, 0, 0, -1, -1, -1, -1 }, //"..A.."1
			{ 1, /* */0, 0, 1, 0, -1, -1, -1, -1, -1 }, //"..A."1
			{ 0, /* */0, 1, 0, -1, -1, -1, -1, -1, -1 }, //".A."0

			/* okay... so, just block something */
			{ 1, /* */0, 0, 2, 0, -1, -1, -1, -1, -1 }, //"..O."1
			{ 0, /* */0, 2, 0, -1, -1, -1, -1, -1, -1 }, //".O."0
			};

	m_Patterns = new char *[STRATEGIES];
	m_Moves = new int[STRATEGIES];

	for (int i = 0; i < STRATEGIES; i++) {
		m_Patterns[i] = new char[PATTERN_LENGTH];
		memset(m_Patterns[i], 0, PATTERN_LENGTH);

		m_Moves[i] = strategy[i][0];

		for (int j = 0; j < PATTERN_LENGTH - 1; j++) {
			switch (strategy[i][1 + j]) {
			case 1:
				m_Patterns[i][j] = mark;
				break;
			case 2:
				m_Patterns[i][j] = opponentMark;
				break;
			case 0:
				m_Patterns[i][j] = CBoard::EMPTY_CELL;
				break;
			case -1:
				m_Patterns[i][j] = '\0';
				break;
			default:
				break;
			}
		}
	}
}

/******************************************************************************/

CAI::~CAI(void) {
	for (int i = 0; i < STRATEGIES; i++) {
		delete[] m_Patterns[i];
	}

	delete[] m_Patterns;
	delete[] m_Moves;
}

/******************************************************************************/

void CAI::nextMove(void) {

	/* nothing to do here */
	if (m_IsWinner == true) {
		return;
	}

	int x = 0;
	int y = 0;
	int direction = 0;

	char message[CBoard::MESSAGE_LENGTH];

	for (int i = 0; i < STRATEGIES; i++) {

		memset(message, 0, CBoard::MESSAGE_LENGTH);
		sprintf(message, " AI (%c) think about: %s", m_Mark, m_Patterns[i]);
		m_Board->printMessage(message);

		usleep(100000);

		if (m_Board->searchFor(x, y, direction, m_Patterns[i])) {

			/* AI can make mistake*/
			if ((rand() % (42 - i)) == 0) {
				memset(message, 0, CBoard::MESSAGE_LENGTH);
				sprintf(message, " AI (%c): Oh no!", m_Mark);
				m_Board->printMessage(message);
				usleep(1500000);

				memset(message, 0, CBoard::MESSAGE_LENGTH);
				sprintf(message, " AI (%c): I made a mistake! :(", m_Mark);
				m_Board->printMessage(message);
				usleep(1500000);

				continue;
			}

			for (int j = 0; j < m_Moves[i]; j++) {
				x += CBoard::DIRECTIONS[direction][0];
				y += CBoard::DIRECTIONS[direction][1];

			}

			if (m_Board->addStone(m_Mark, x, y)) {
				return;
			}
		}
	}

	memset(message, 0, CBoard::MESSAGE_LENGTH);
	sprintf(message, " AI (%c): I go random.", m_Mark);
	m_Board->printMessage(message);
	usleep(1000000);

	goRandom();
}
