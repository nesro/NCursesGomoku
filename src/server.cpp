/**
 * @file server.cpp
 * @brief Network opponent as server.
 * 
 * @author Tomas Nesrovnal nesro@nesro.cz
 * @version 1.0
 */

#include <ncurses.h>

/******************************************************************************/

#include "server.hpp"

/******************************************************************************/

CServer::CServer(CBoard * board, char mark, CGame * game) :
CPlayer(board, mark), CNetwork(game) {

	this->m_ClientLength = 0;
	this->m_ClientSocketFileDescriptor = 0;

    /* connect to server only once */
    if (m_Game->m_IsServerUp == true) {
        return;
    } else {
        m_Game->m_IsServerUp = true;
        m_Game->m_IsConnectionUp = true;
    }

    sockaddr_in sockName;

    if ((m_Game->m_ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) ==
            -1) {
        throw CNetworkException("Fatal network error: Server socket failed.");
    }

    int intWithNumberOne = 1;
    if (setsockopt(m_Game->m_ServerSocket, SOL_SOCKET, SO_REUSEADDR,
            (char *) & intWithNumberOne, sizeof (intWithNumberOne)) < 0) {
        close(m_Game->m_ServerSocket);
        throw CNetworkException("Fatal network error: Server REUSEADDR fail.");
    }

    sockName.sin_family = AF_INET;
    sockName.sin_port = htons(m_Game->m_Port);
    sockName.sin_addr.s_addr = INADDR_ANY;

    if (bind(m_Game->m_ServerSocket, (sockaddr *) & sockName, sizeof (sockName))
            == -1) {
        throw CNetworkException("Fatal network error: Server bind failed. \
Program maybe was killed and port was not closed. Please, rerun program with \
another port.");
    }

    if (listen(m_Game->m_ServerSocket, 10) == -1) {
        throw CNetworkException("Fatal network error: Server listen failed.");
    }


    sockaddr_in clientInfo;
    socklen_t addrlen;

    addrlen = sizeof (clientInfo);

    m_Game->clearScreen();

    mvprintw(0, 0, "==============================");
    mvprintw(1, 0, "   >> Waiting for client <<   ");
    mvprintw(2, 0, "==============================");
    refresh();

    m_Game->m_ConnectionSocket = accept(m_Game->m_ServerSocket,
            (sockaddr *) & clientInfo, &addrlen);
}

/******************************************************************************/

CServer::~CServer(void) {

}

/******************************************************************************/

void CServer::nextMove(void) {
    int sendSize;
    char sendData[4];
    memset(sendData, '\0', 4);

    int recvSize;
    char recvData[4];
    memset(recvData, '\0', 4);

    char message[CBoard::MESSAGE_LENGTH];

    /* prepare send data */
    if (m_Game->m_Players[0]->m_Surrender) {
        sendData[2] = (char) 1;
    } else {
        sendData[0] = (char) m_Board->m_LastStone.m_X;
        sendData[1] = (char) m_Board->m_LastStone.m_Y;
    }

    /* send */
    memset(message, 0, CBoard::MESSAGE_LENGTH);
    sprintf(message, " Server sending: %d,%d,%d", sendData[0],
            sendData[1], sendData[2]);
    m_Board->printMessage(message);
    usleep(1000000);


    if ((sendSize = send(m_Game->m_ConnectionSocket, sendData, 4, 0)) == -1) {
        throw CNetworkException("Fatal network error: Server send failed.");
    }

    /* nothing to do here */
    if (m_Game->m_Players[0]->m_Surrender || m_IsWinner == true) {
        return;
    }

    /* recv */
    for (int i = 0; i < 10; i++) {

        m_Board->printMessage(" Server waits for client move.");
        usleep(500000);

        if ((recvSize = recv(m_Game->m_ConnectionSocket, recvData, 4, 0)) ==
                -1) {
            throw CNetworkException("Fatal network error: Server recv failed.");
        }

        /* parse */
        memset(message, 0, CBoard::MESSAGE_LENGTH);
        sprintf(message, " Server received: %d,%d,%d", recvData[0],
                recvData[1], recvData[2]);
        m_Board->printMessage(message);

        usleep(500000);

        if (recvData[0] == -1 || recvData[1] == -1) {
            continue;
        }

        if (recvData[2] == 1) {
            m_Surrender = true;
            return;
        }

        if (m_Board->addStone(m_Mark, (int) recvData[1], (int) recvData[0]) ==
                false) {
            throw CNetworkException("Fatal network error: Bad data recevied.");
        } else {
            return;
        }
    }
    throw CNetworkException("Fatal network error: Please, wait and then \
run again program with another port.");
}
