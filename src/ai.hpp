/**
 * @file ai.hpp
 * @brief Header file for artificial intelligence opponent.
 * 
 * @author Tomas Nesrovnal nesro@nesro.cz
 * @version 1.0
 */

#include "game.hpp"
#include "board.hpp"
#include "player.hpp"

/******************************************************************************/

#ifndef __AI_HPP__

/**
 * For safety about multiple include
 */
#define	__AI_HPP__

/**
 * @brief An artificial intelligence opponent.
 */
class CAI : public CPlayer {
public:

    /**
     * Length of the pattern to search for in the gameboard.
     */
    static const int PATTERN_LENGTH = 10;
    
    /**
     * How many patterns for search.
     */
    static const int STRATEGIES = 21;

    /**
     * Create the AI patterns to search for.
     * @param board Game board
     * @param mark Mark of player
     */
    CAI(CBoard *board, char mark);

    /**
     * Destructor. Cleans up the allocated objects.
     */
    ~CAI(void);

    /**
     * Search and do move.
     */
    void nextMove(void);
    
protected:
    
    /**
     * An 2D array for computed patterns to search for.
     */
    char **m_Patterns;
    
    /**
     * How many steps to do, when the pattern is found.
     */
    int *m_Moves;
};

#endif	/* __AI_HPP__ */

