/**
 * @file server.hpp
 * @brief Header file for network opponent as server.
 * 
 * @author Tomas Nesrovnal nesro@nesro.cz
 * @version 1.0
 */

#include "player.hpp"
#include "local.hpp"
#include "board.hpp"
#include "network.hpp"
#include "game.hpp"

/******************************************************************************/

#ifndef __SERVER_HPP__

/**
 * For safety about multiple include
 */
#define	__SERVER_HPP__

/**
 * @brief Network opponent as server.
 */
class CServer : public CPlayer, public CNetwork {
public:

    /**
     * Initialize server a network connection
     * @param board Game board
     * @param mark Player mark
     * @param game Game object
     */
    CServer(CBoard * board, char mark, CGame * game);

    /**
     * Close a network connection.
     */
    ~CServer(void);

    /**
     * Wait for move of a network opponent.
     */
    virtual void nextMove(void);

protected:

    /**
     * Size of a client socket
     */
    socklen_t m_ClientLength;
    
    /**
     * File descriptor for a client connection.
     */
    int m_ClientSocketFileDescriptor;
};

#endif	/* __SERVER_HPP__ */
