/**
 * @file board.hpp
 * @brief Header file for game board and board functionality like search patterns.
 * 
 * @author Tomas Nesrovnal nesro@nesro.cz
 * @version 1.0
 */

#ifndef __BOARD_HPP__

/**
 * For safety about multiple include
 */
#define	__BOARD_HPP__

/**
 * @brief Class holding the game board and board functionality like
 * a search patterns.
 */
class CBoard {
public:

	/**
	 * Horizontal width of the board.
	 */
	static const int BOARD_WIDTH = 15;

	/**
	 * Vertical height of the board.
	 */
	static const int BOARD_HEIGHT = 15;

	/**
	 * Numerial expression for direction up.
	 */
	static const int DIRECTION_UP = 0;

	/**
	 * Numerial expression for direction left.
	 */
	static const int DIRECTION_LEFT = 1;

	/**
	 * Numerial expression for direction down.
	 */
	static const int DIRECTION_DOWN = 2;

	/**
	 * Numerial expression for direction right.
	 */
	static const int DIRECTION_RIGHT = 3;

	/**
	 * How is displayed an empty cell in the game board.
	 */
	static const char EMPTY_CELL = '_';

	/**
	 * Length of the message space on the game board.
	 */
	static const int MESSAGE_LENGTH = 32;

	/**
	 * Direction in all eight ways in the 2D game board.
	 */
	static const int DIRECTIONS[8][2];

	/**
	 * Constructor.
	 */
	CBoard(void);

	/**
	 * Destructor.
	 */
	~CBoard(void);

	/**
	 * Prints game board on output.
	 */
	void show(void) const;

	/**
	 * Add stone on game board.
	 * @param mark Mark
	 * @param x X
	 * @param y Y
	 * @return false if cell is not empty
	 */
	bool addStone(char mark, int x, int y);

	/**
	 * Move the cursor in one of eight directions.
	 * @param direction Direction.
	 */
	void moveCursor(int direction);

	/**
	 * \brief Struct holding x a y coords.
	 */
	struct TCoord {
		/**
		 * X coord.
		 */
		int m_X;

		/**
		 * Y coord.
		 */
		int m_Y;
	};

	/**
	 * Are x and y coords out of the board?
	 * @param x X
	 * @param y Y
	 * @return true if coords are out of board
	 */
	bool outOfBoard(int x, int y) const;

	/**
	 * Search in the board for a pattern.
	 * @param x X to search
	 * @param y Y to search
	 * @param direction Direction to search
	 * @param pattern Pattern to search
	 * @return true, if pattern was found
	 */

	bool searchFor(int & x, int & y, int & direction,
			const char * pattern) const;

	/**
	 * Prints a message to the game board
	 * @param message Message to print
	 */
	void printMessage(const char * message);

	/**
	 * Reset the board and sets the variables default.
	 */
	void reset(void);

	/**
	 * String with a message to print.
	 */
	char m_Message[MESSAGE_LENGTH + 1];

	/**
	 * How many stones are on the board.
	 */
	int m_StonesOnBoard;

	/**
	 * Where was placed the last stone.
	 */
	TCoord m_LastStone;

	/**
	 * Position of the cursor on the game board.
	 */
	TCoord m_Cursor;

protected:

	/**
	 * Where are stones on the game board and where are empty cells.
	 */
	char m_Stones[BOARD_WIDTH][BOARD_HEIGHT];

	/**
	 *  Minimal cursor position.
	 */
	static const int CURSOR_MIN = 3;

	/**
	 * Maximal cursor position
	 */
	static const int CURSOR_MAX = 17;

	/**
	 * Position of cursor start
	 */
	static const int CURSOR_START = 5;

};

#endif	/* __BOARD_HPP__ */
