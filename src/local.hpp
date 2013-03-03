/**
 * @file local.hpp
 * @brief Header file for player that plays with keyboard.
 * 
 * @author Tomas Nesrovnal nesro@nesro.cz
 * @version 1.0
 */

#include "game.hpp"
#include "player.hpp"

/******************************************************************************/

#ifndef __LOCAL_HPP__

/**
 * For safety about multiple include
 */
#define	__LOCAL_HPP__

/**
 * @brief Local player that plays with a keyboard.
 */
class CLocal : public CPlayer {
public:

    /**
     * Ininitialize a player.
     * @param board Pointer to the game board
     * @param mark Mark of the player
     */
    CLocal(CBoard * board, char mark) : CPlayer(board, mark) {
    }

    /**
     * Free player.
     */
    ~CLocal(void) {
    }

    /**
     * Read input from a keyboard and make move.
     */
    void nextMove(void);

};

#endif	/* __LOCAL_HPP__ */
