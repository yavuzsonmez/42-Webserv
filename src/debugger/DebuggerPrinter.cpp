#include "../../inc/DebuggerPrinter.hpp"

DebuggerPrinter::DebuggerPrinter()
{

}
DebuggerPrinter::DebuggerPrinter( const DebuggerPrinter &src )
{

}

DebuggerPrinter::~DebuggerPrinter()
{

}

DebuggerPrinter & DebuggerPrinter::operator = (const DebuggerPrinter &src) {
	return (*this);
}

/**
 * @brief Prints message with color code and level prefix
 * 
 * @param message 
 */
void DebuggerPrinter::printString( const std::string &message, LogLevel level ) {
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
		default:
			prefix = "[UNKNOWN]";
	}
	std::cout << prefix << ":" << message << std::endl;
}

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
	printString(message, DEBUG);
}
