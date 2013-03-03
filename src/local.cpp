/**
 * @file local.cpp
 * @brief Player that plays with keyboard.
 * 
 * @author Tomas Nesrovnal nesro@nesro.cz
 * @version 1.0
 */

#include <ncurses.h>
#include <cstring>

/******************************************************************************/

#include "local.hpp"

/******************************************************************************/

void CLocal::nextMove(void) {

    /* nothing to do here */
    if (m_IsWinner == true) {
        return;
    }

    int key;

    char message[CBoard::MESSAGE_LENGTH];
    memset(message, '\0', CBoard::MESSAGE_LENGTH);
    sprintf(message, " Your (%c) turn!", m_Mark);
    m_Board->printMessage(message);

    /* make cursor visible */
    curs_set(2);

    for (;;) {
        key = getch();

        switch (key) {
            case KEY_UP:
                m_Board->moveCursor(CBoard::DIRECTION_UP);
                break;
                
            case KEY_DOWN:
                m_Board->moveCursor(CBoard::DIRECTION_DOWN);
                break;
                
            case KEY_LEFT:
                m_Board->moveCursor(CBoard::DIRECTION_LEFT);
                break;
                
            case KEY_RIGHT:
                m_Board->moveCursor(CBoard::DIRECTION_RIGHT);
                break;
                
            case 'w':
            case 'W':
            case ' ':
                if (m_Board->addStone(m_Mark, -1, -1)) {
                    curs_set(0);
                    return;
                }
                
                break;
                
            case 'q':
            case 'Q':
                m_Surrender = true;

                curs_set(0);
                return;

                break;
                
            default:
                break;
        }
    }
}
