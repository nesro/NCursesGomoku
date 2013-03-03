/**
 * @file client.hpp
 * @brief Header file for network opponent as client.
 * 
 * @author Tomas Nesrovnal nesro@nesro.cz
 * @version 1.0
 */

#include "local.hpp"
#include "board.hpp"
#include "network.hpp"
#include "game.hpp"

/******************************************************************************/

#ifndef __CLIENT_HPP__

/**
 * For safety about multiple include
 */
#define	__CLIENT_HPP__

/**
 * @brief Network opponent as client.
 */
class CClient : public CPlayer, public CNetwork {
public:

    /**
     * Initialize a client netwok conection
     * @param board Game board
     * @param mark Player mark
     * @param game Game object
     */
    CClient(CBoard * board, char mark, CGame * game);

    /**
     * Close the network connection.
     */
    ~CClient(void);

    /**
     * Wait for move of a network opponent.
     */
    virtual void nextMove(void);

};

#endif	/* __CLIENT_HPP__ */
