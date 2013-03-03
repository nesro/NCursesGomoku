/**
 * @file network_exception.hpp
 * @brief Header file for providing nice error output.
 * 
 * @author Tomas Nesrovnal nesro@nesro.cz
 * @version 1.0
 */

#include <iostream>
#include <exception>

/******************************************************************************/

#ifndef __NETWORK_EXCEPTION_HPP__

/**
 * For safety about multiple include
 */
#define	__NETWORK_EXCEPTION_HPP__

/**
 * @brief Exception class used when is a error in a network comunication.
 */
class CNetworkException: public std::exception {
public:

	/**
	 * Save text to throw.
	 * @param text Exception text
	 */
	CNetworkException(const char *text);

	/**
	 * Destructor.
	 */
	~CNetworkException(void) throw ();

	/**
	 * Returning text to throw.
	 * @return Text
	 */
	virtual const char *what(void) const throw ();

protected:

	/**
	 * String to throw.
	 */
	std::string m_Text;
};

#endif	/* __NETWORK_EXCEPTION_HPP__ */
