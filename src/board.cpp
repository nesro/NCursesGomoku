/**
 * @file board.cpp
 * @brief Game board and board functionality like search patterns.
 * 
 * @author Tomas Nesrovnal nesro@nesro.cz
 * @version 1.0
 */

#include <ncurses.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

/******************************************************************************/

#include "game.hpp"
#include "board.hpp"

/******************************************************************************/

const int CBoard::DIRECTIONS[8][2] = { { -1, 1 }, { 1, -1 }, { 0, -1 },
		{ 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 0 }, { -1, -1 } };

/******************************************************************************/

CBoard::CBoard() {
	reset();
}

/******************************************************************************/

CBoard::~CBoard(void) {

}

/******************************************************************************/

void CBoard::reset(void) {
	for (int i = 0; i < BOARD_WIDTH; i++) {
		for (int j = 0; j < BOARD_HEIGHT; j++) {
			m_Stones[i][j] = EMPTY_CELL;
		}
	}

	memset(m_Message, 0, MESSAGE_LENGTH + 1);

	m_LastStone.m_X = -1;
	m_LastStone.m_Y = -1;

	m_StonesOnBoard = 0;

	m_Cursor.m_X = (int) ((CURSOR_MAX + CURSOR_MIN) / 2);
	m_Cursor.m_Y = (int) ((CURSOR_MAX + CURSOR_MIN) / 2);
}

/******************************************************************************/

void CBoard::show(void) const {

	int row = 0;
	int col = 0;

	mvprintw(row++, col, "/=====================================\\");
	mvprintw(row++, col, "|    a b c d e f g h i j k l m n o    |");
	mvprintw(row++, col,
			"|    _ _ _ _ _ _ _ _ _ _ _ _ _ _ _    |\\=================\\");
	mvprintw(row++, col,
			"|00 |_|_|_|_|_|_|_|_|_|_|_|_|_|_|_| 00|                  |");
	mvprintw(row++, col,
			"|01 |_|_|_|_|_|_|_|_|_|_|_|_|_|_|_| 01| Controls:        |");
	mvprintw(row++, col,
			"|02 |_|_|_|_|_|_|_|_|_|_|_|_|_|_|_| 02|                  |");
	mvprintw(row++, col,
			"|03 |_|_|_|_|_|_|_|_|_|_|_|_|_|_|_| 03| w or ' ' = write |");
	mvprintw(row++, col,
			"|04 |_|_|_|_|_|_|_|_|_|_|_|_|_|_|_| 04| q = surrender    |");
	mvprintw(row++, col,
			"|05 |_|_|_|_|_|_|_|_|_|_|_|_|_|_|_| 05| arrows = move    |");
	mvprintw(row++, col,
			"|06 |_|_|_|_|_|_|_|_|_|_|_|_|_|_|_| 06|                  |");
	mvprintw(row++, col,
			"|07 |_|_|_|_|_|_|_|_|_|_|_|_|_|_|_| 07|/=================/");
	mvprintw(row++, col, "|08 |_|_|_|_|_|_|_|_|_|_|_|_|_|_|_| 08|");
	mvprintw(row++, col, "|09 |_|_|_|_|_|_|_|_|_|_|_|_|_|_|_| 09|");
	mvprintw(row++, col, "|10 |_|_|_|_|_|_|_|_|_|_|_|_|_|_|_| 10|");
	mvprintw(row++, col, "|11 |_|_|_|_|_|_|_|_|_|_|_|_|_|_|_| 11|");
	mvprintw(row++, col, "|12 |_|_|_|_|_|_|_|_|_|_|_|_|_|_|_| 12|");
	mvprintw(row++, col, "|13 |_|_|_|_|_|_|_|_|_|_|_|_|_|_|_| 13|");
	mvprintw(row++, col, "|14 |_|_|_|_|_|_|_|_|_|_|_|_|_|_|_| 14|");
	mvprintw(row++, col, "|    a b c d e f g h i j k l m n o    |");
	mvprintw(row++, col, "|    0 0 0 0 0 0 0 0 0 0 1 1 1 1 1    |");
	mvprintw(row++, col, "|    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4    |");
	mvprintw(row++, col, "\\=====================================/");
	mvprintw(row++, col + 1, "\\=[                               ]=/");
	mvprintw(row++, col + 2, "\\=================================/");

	for (int i = 0; i < BOARD_WIDTH; i++) {
		for (int j = 0; j < BOARD_HEIGHT; j++) {
			if (m_Stones[i][j] == CGame::PLAYER_0_CHAR) {
				if (m_LastStone.m_X == i && m_LastStone.m_Y == j) {
					attron(COLOR_PAIR(4));

					mvprintw(CURSOR_MIN + i, CURSOR_START + j * 2, "%c",
							CGame::PLAYER_0_CHAR_BIG);

					attroff(COLOR_PAIR(4));
				} else {
					attron(COLOR_PAIR(2));

					mvprintw(CURSOR_MIN + i, CURSOR_START + j * 2, "%c",
							m_Stones[i][j]);

					attroff(COLOR_PAIR(2));
				}
			} else if (m_Stones[i][j] == CGame::PLAYER_1_CHAR) {
				if (m_LastStone.m_X == i && m_LastStone.m_Y == j) {
					attron(COLOR_PAIR(5));

					mvprintw(CURSOR_MIN + i, CURSOR_START + j * 2, "%c",
							CGame::PLAYER_1_CHAR_BIG);

					attroff(COLOR_PAIR(5));
				} else {
					attron(COLOR_PAIR(3));

					mvprintw(CURSOR_MIN + i, CURSOR_START + j * 2, "%c",
							m_Stones[i][j]);

					attroff(COLOR_PAIR(3));
				}
			}
		}
	}

	mvprintw(22, 4, "%s", m_Message);

	move(m_Cursor.m_Y, m_Cursor.m_Y * 2 - 1);
}

/******************************************************************************/

bool CBoard::addStone(char mark, int x, int y) {

	if (x == -1) {
		x = m_Cursor.m_X - CURSOR_MIN;
	}

	if (y == -1) {
		y = m_Cursor.m_Y - CURSOR_MIN;
	}

	if (m_Stones[y][x] == EMPTY_CELL) {
		m_StonesOnBoard++;

		m_LastStone.m_X = y;
		m_LastStone.m_Y = x;

		m_Stones[y][x] = mark;
	} else {
		return false;
	}

	return true;
}

/******************************************************************************/

void CBoard::moveCursor(int direction) {
	switch (direction) {
	case DIRECTION_UP:
		m_Cursor.m_Y--;
		if (m_Cursor.m_Y < CURSOR_MIN) {
			m_Cursor.m_Y = CURSOR_MAX;
		}
		break;
	case DIRECTION_DOWN:
		m_Cursor.m_Y++;
		if (m_Cursor.m_Y > CURSOR_MAX) {
			m_Cursor.m_Y = CURSOR_MIN;
		}
		break;
	case DIRECTION_LEFT:
		m_Cursor.m_X--;
		if (m_Cursor.m_X < CURSOR_MIN) {
			m_Cursor.m_X = CURSOR_MAX;
		}
		break;
	case DIRECTION_RIGHT:
		m_Cursor.m_X++;
		if (m_Cursor.m_X > CURSOR_MAX) {
			m_Cursor.m_X = CURSOR_MIN;
		}
		break;
	default:
		break;
	}

	move(m_Cursor.m_Y, m_Cursor.m_X * 2 - 1);
}

/******************************************************************************/

bool CBoard::outOfBoard(int x, int y) const {
	return (0 > x || 0 > y || x >= BOARD_WIDTH || y >= BOARD_HEIGHT);
}

/******************************************************************************/

bool CBoard::searchFor(int & x, int & y, int & direction,
		const char * pattern) const {

	int randomSearch = rand() % 4;

	/* choose one from four directions of searching */
	switch (randomSearch) {
	case 0:
		for (int i = 0; i < BOARD_WIDTH; i++) {
			for (int j = 0; j < BOARD_HEIGHT; j++) {
				for (int searchDirection = 0; searchDirection < 8;
						searchDirection++) {
					for (int l = 0, tmpX = i, tmpY = j;; l++) {
						if (pattern[l] == '\0') {
							x = i;
							y = j;
							direction = searchDirection;

							return true;
						} else if (!outOfBoard(tmpX, tmpY)
								&& m_Stones[tmpY][tmpX] == pattern[l]) {
							tmpX += DIRECTIONS[searchDirection][0];
							tmpY += DIRECTIONS[searchDirection][1];

							continue;
						} else {
							break;
						}
					}
				}
			}
		}
		break;
	case 1:
		for (int i = BOARD_WIDTH - 1; i >= 0; i--) {
			for (int j = 0; j < BOARD_HEIGHT; j++) {
				for (int searchDirection = 0; searchDirection < 8;
						searchDirection++) {
					for (int l = 0, tmpX = i, tmpY = j;; l++) {
						if (pattern[l] == '\0') {
							x = i;
							y = j;
							direction = searchDirection;

							return true;
						} else if (!outOfBoard(tmpX, tmpY)
								&& m_Stones[tmpY][tmpX] == pattern[l]) {
							tmpX += DIRECTIONS[searchDirection][0];
							tmpY += DIRECTIONS[searchDirection][1];

							continue;
						} else {
							break;
						}
					}
				}
			}
		}
		break;
	case 2:
		for (int i = 0; i < BOARD_WIDTH; i++) {
			for (int j = BOARD_HEIGHT - 1; j >= 0; j--) {
				for (int searchDirection = 0; searchDirection < 8;
						searchDirection++) {
					for (int l = 0, tmpX = i, tmpY = j;; l++) {
						if (pattern[l] == '\0') {
							x = i;
							y = j;
							direction = searchDirection;

							return true;
						} else if (!outOfBoard(tmpX, tmpY)
								&& m_Stones[tmpY][tmpX] == pattern[l]) {
							tmpX += DIRECTIONS[searchDirection][0];
							tmpY += DIRECTIONS[searchDirection][1];

							continue;
						} else {
							break;
						}
					}
				}
			}
		}
		break;
	case 3:
		for (int i = BOARD_WIDTH - 1; i >= 0; i--) {
			for (int j = BOARD_HEIGHT - 1; j >= 0; j--) {
				for (int searchDirection = 0; searchDirection < 8;
						searchDirection++) {
					for (int l = 0, tmpX = i, tmpY = j;; l++) {
						if (pattern[l] == '\0') {
							x = i;
							y = j;
							direction = searchDirection;

							return true;
						} else if (!outOfBoard(tmpX, tmpY)
								&& m_Stones[tmpY][tmpX] == pattern[l]) {
							tmpX += DIRECTIONS[searchDirection][0];
							tmpY += DIRECTIONS[searchDirection][1];

							continue;
						} else {
							break;
						}
					}
				}
			}
		}
		break;
	default:
		break;
	}

	return false;
}

/******************************************************************************/

void CBoard::printMessage(const char * message) {
	mvprintw(22, 4, "                               ");
	strcpy(m_Message, message);
	mvprintw(22, 4, "%s", m_Message);
	move(m_Cursor.m_Y, m_Cursor.m_X * 2 - 1);
	refresh();
}
