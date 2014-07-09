/**
 * @file main.cpp
 * @brief Main file of program.
 * 
 * @author Tomas Nesrovnal nesro@nesro.cz
 * @version 1.0
 *
 * \mainpage "Piskvorky" doxygen documentation
 * 
 * @section About
 * 
 * Welcome to documentation for "piskvorky". It's a
 * simple game written in C\\C++. It's a variant of the "five in row" game. It
 * uses the ncurses library for graphic in ASCII art and the socket library for
 * a network communication.
 * 
 * <hr>
 * 
 * @section Screenshot
 * 
 * @image html screenshot.png
 * 
 * <hr>
 * 
 * @section Polymorphysm
 * 
 * @code
 *     m_Players[m_PlayerOnTurn]->nextMove();
 * @endcode
 * 
 */

#include <iostream>
#include <exception>
#include <cstdlib>

/******************************************************************************/

#include "game.hpp"
#include "network_exception.hpp"

/******************************************************************************/

/**
 * Main function of program.
 * @param argc Program argument count
 * @param argv Program arguments
 */
int main(int argc, char *argv[]) {
	CGame *game = NULL;
	try {
		game = new CGame(argc, argv);
		for (;;) {
			switch (game->m_Status) {
			case CGame::STATUS_TURN:
				game->playerMove();
				game->renderBoard();
				game->checkForWin();

				break;
			case CGame::STATUS_MENU:
				game->showMenu();

				break;
			case CGame::STATUS_EXIT:
				game->networkCleanup();
				delete game;
				return EXIT_SUCCESS;
			default:
				break;
			}
		}
	} catch (std::exception & e) {
		delete game;
		std::cerr << "Exception: " << e.what() << std::endl;
	}

	return EXIT_FAILURE;
}
