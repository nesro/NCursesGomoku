/**
 * @file network.hpp
 * @brief Header file for base for network players.
 * 
 * @author Tomas Nesrovnal nesro@nesro.cz
 * @version 1.0
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h> 

/******************************************************************************/

#include "game.hpp"
#include "network_exception.hpp"

/******************************************************************************/

#ifndef __NETWORK_HPP__

/**
 * For safety about multiple include
 */
#define	__NETWORK_HPP__

/**
 * @brief Base class for network players.
 */
class CNetwork {
public:

    /**
     * Default port to connect.
     */
    static const int DEFAULT_PORT = 1101;
    
    /**
     * Initialize variables for a network connection
     * @param game Pointer to a game object
     */
    CNetwork(CGame *game);

    /**
     * Free variables.
     */
    ~CNetwork(void);
    
protected:

    /**
     * Port number
     */
    int m_Port;
    
    /**
     * Pointer to a game object
     */
    CGame *m_Game;
};

#endif	/* __NETWORK_HPP__ */
