/**
 * @file player.cpp
 * @brief Base for players.
 * 
 * @author Tomas Nesrovnal nesro@nesro.cz
 * @version 1.0
 */

#include <cstdlib>

/******************************************************************************/

#include "player.hpp"

/******************************************************************************/

CPlayer::CPlayer(CBoard * board, char mark) : m_Mark(mark), m_Board(board) {
    m_Surrender = false;
    m_IsWinner = false;
}

/******************************************************************************/

CPlayer::~CPlayer(void) {
}

/******************************************************************************/

void CPlayer::goRandom(void) const {
    int middle = 15 / 2;
    int randomRange = 1;

    /* search for first empty stone in from middle of board */
    while (m_Board->addStone(m_Mark, middle - randomRange + rand() %
            (randomRange * 2), middle - randomRange + rand() % (randomRange * 2)
            ) == false) {
        if (randomRange < 8) {
            randomRange++;
        }
    }
}
