/**
 * @file network.cpp
 * @brief Base for network players.
 * 
 * @author Tomas Nesrovnal nesro@nesro.cz
 * @version 1.0
 */

#include "game.hpp"
#include "network.hpp"

/******************************************************************************/

CNetwork::CNetwork(CGame *game) {
    this->m_Game = game;
    this->m_Port = -1;
}

/******************************************************************************/

CNetwork::~CNetwork(void) {

}
