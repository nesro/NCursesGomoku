/**
 * @file player.hpp
 * @brief Header file for base for players.
 * 
 * @author Tomas Nesrovnal nesro@nesro.cz
 * @version 1.0
 */

#include "board.hpp"

/******************************************************************************/

#ifndef __PLAYER_HPP__

/**
 * For safety about multiple include
 */
#define __PLAYER_HPP__

/**
 * @brief Base class for players.
 */
class CPlayer {
public:

    /**
     * Initialize player variables.
     * @param board Game board
     * @param mark Mark of player
     */
    CPlayer(CBoard * board, char mark);

    /**
     * Destructor.
     */
    virtual ~CPlayer(void);
    
    /**
     * Make next move on game board.
     */
    virtual void nextMove(void) = 0;

    /**
     * If true, player is surrender.
     */
    bool m_Surrender;

    /**
     * Search in random order for first free cell in middle of game board.
     */
    void goRandom(void) const;

    /**
     * Mark for player on game board. ("x" or "o")
     */
    char m_Mark;

    /**
     * Is any winner found?
     */
    bool m_IsWinner;

protected:

    /**
     * Pointer to game board.
     */
    CBoard * m_Board;
    
};

#endif /* __PLAYER_HPP__ */
