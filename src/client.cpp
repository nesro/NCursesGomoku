/**
 * @file client.cpp
 * @brief Network opponent as client.
 * 
 * @author Tomas Nesrovnal nesro@nesro.cz
 * @version 1.0
 */

#include "client.hpp"
#include "player.hpp"

/******************************************************************************/

CClient::CClient(CBoard * board, char mark, CGame * game) :
CPlayer(board, mark), CNetwork(game) {

    if (m_Game->m_IsConnectionUp == true) {
        return;
    } else {
        m_Game->m_IsConnectionUp = true;
    }

    hostent * host;

    if ((host = gethostbyname(m_Game->m_Host.c_str())) == NULL) {
        throw CNetworkException("Fatal network error: Client host is invalid.");
    }

    if ((m_Game->m_ConnectionSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))
            == -1) {
        throw CNetworkException("Fatal network error: Client socket error.");
    }

    sockaddr_in serverSock;

    serverSock.sin_family = AF_INET;
    serverSock.sin_port = htons(m_Game->m_Port);
    memcpy(&(serverSock.sin_addr), host->h_addr, host->h_length);

    if (connect(m_Game->m_ConnectionSocket, (sockaddr *) & serverSock,
            sizeof (serverSock)) == -1) {
        throw CNetworkException("Fatal network error: Client connect failed.");
    }
}

/******************************************************************************/

CClient::~CClient(void) {

}

/******************************************************************************/

void CClient::nextMove(void) {
    
    static bool firstMove = true;

    int sendSize;
    char sendData[4];
    memset(sendData, '\0', 4);

    int recvSize;
    char recvData[4];
    memset(recvData, '\0', 4);

    char message[CBoard::MESSAGE_LENGTH];

    /* if this is the first move of the client, he can not send the data about
     * the local player move, because he don't play yet. */
    if (firstMove == false) {

        /* prepare send data */
        if (m_Game->m_Players[0]->m_Surrender) {
            sendData[2] = (char) 1;
        } else {
            sendData[0] = (char) m_Board->m_LastStone.m_X;
            sendData[1] = (char) m_Board->m_LastStone.m_Y;
        }

        /* send */
        m_Board->printMessage(" Client sending informations.");
        usleep(500000);

        if ((sendSize = send(m_Game->m_ConnectionSocket, sendData, 4, 0)) ==
                -1) {
            throw CNetworkException("Fatal network error: Client send failed.");
        }

    } else {
        firstMove = false;
    }

    /* nothing to do here */
    if (m_Game->m_Players[0]->m_Surrender || m_IsWinner == true) {
        return;
    }

    /* recv */
    m_Board->printMessage(" Client waits for server move.");
    usleep(500000);

    if ((recvSize = recv(m_Game->m_ConnectionSocket, recvData, 4, 0)) == -1) {
        throw CNetworkException("Fatal network error: Client recv failed.");
    }

    /* parse */
    memset(message, 0, CBoard::MESSAGE_LENGTH);
    sprintf(message, " Client received: %d,%d,%d", recvData[0],
            recvData[1], recvData[2]);
    m_Board->printMessage(message);

    usleep(500000);

    if (recvData[2] == 1) {
        m_Surrender = true;
        return;
    }

    if (m_Board->addStone(m_Mark, (int) recvData[1], (int) recvData[0]) ==
            false) {
        throw CNetworkException("Fatal network error: Bad data received.");
    }
}
