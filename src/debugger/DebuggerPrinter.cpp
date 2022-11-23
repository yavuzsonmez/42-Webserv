#include "../../inc/debugger/DebuggerPrinter.hpp"
#include <string>

DebuggerPrinter::DebuggerPrinter()
{

}

DebuggerPrinter::~DebuggerPrinter()
{

}

DebuggerPrinter & DebuggerPrinter::operator = (const DebuggerPrinter &src) {
	(void) src;
	return (*this);
}

/**
 * @brief Prints message with color code and level prefix
 *
 * @param message
 */
void DebuggerPrinter::printString( const std::string &message, LogLevel level ) {
	if (ENABLE_LOGGING == 0) return ;
	std::string prefix = "";
	switch (level) {
		case INFO:
			prefix = "\033[0;34m[INFO]\033[0m";
			break;
		case WARNING:
			prefix = "\033[0;31m[WARNING]\033[0m";
			break;
		case DEBUG:
			prefix = "\033[0;36m[DEBUG]\033[0m";
			break;
		case VERBOSE:
			prefix = "\033[0;36m[VERBOSE]\033[0m";
			break;
		case ERROR:
			prefix = "\033[0;31m[ERROR]\033[0m";
			break;
		default:
			prefix = "[UNKNOWN]";
	}
	if (level != ERROR)
		std::cout << prefix << ":" << message << std::endl;
	else
		std::cerr << prefix << ":" << message << std::endl;
};

/**
 * @brief Prints out a info message
 *
 * @param message
 */
void DebuggerPrinter::info( const std::string &message ) {
	printString(message, INFO);
}

/**
 * @brief Prints out an warning message
 *
 * @param message
 */
void DebuggerPrinter::warning( const std::string &message ) {
	printString(message, WARNING);
}

/**
 * @brief Prints out an debug message
 *
 * @param message
 */
void DebuggerPrinter::debug( const std::string &message ) {
	if (DEBUGMODE != 1) return ;
	printString(message, DEBUG);
}

void DebuggerPrinter::verbose( const std::string &message ) {
	if (DEBUGMODE != 1) return ;
	printString(message, VERBOSE);
}

/**
 * @brief Prints out an error message
 *
 * @param message
 */
void DebuggerPrinter::error( const std::string &message ) {
	printString(message, ERROR);
}