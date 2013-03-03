/**
 * @file network_exception.cpp
 * @brief Provides nice error output.
 * 
 * @author Tomas Nesrovnal nesro@nesro.cz
 * @version 1.0
 */

#include <iostream>

/******************************************************************************/

#include "network_exception.hpp"

/******************************************************************************/

CNetworkException::CNetworkException(const char *text) : m_Text(text) {
}

/******************************************************************************/

CNetworkException::~CNetworkException(void) throw () {
}

/******************************************************************************/

const char *CNetworkException::what() const throw () {
    return m_Text.c_str();
}
