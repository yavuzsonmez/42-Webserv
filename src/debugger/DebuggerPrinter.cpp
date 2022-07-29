#include "../../inc/DebuggerPrinter.hpp"

DebuggerPrinter::DebuggerPrinter() {

};
DebuggerPrinter::DebuggerPrinter( const DebuggerPrinter &src )
{

}
~DebuggerPrinter::DebuggerPrinter()
{

}
DebuggerPrinter::DebuggerPrinter & operator = (const DebuggerPrinter &src)
{

}

/**
 * @brief Prints message with color code and levek prefix
 * 
 * @param message 
 */
DebuggerPrinter::printString( std::string &message, LogLevel level ) {
	std::string prefix = "";
	switch (level) {
		case LogLevel::INFO:
			prefix = "\033[0;34m[INFO]\033[0m";
			break;
		case LogLevel::WARNING:
			prefix = "\033[0;31m[WARNING]\033[0m";
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
DebuggerPrinter::info( std::string &message ) {
	printString(message, LogLevel::INFO);
}

/**
 * @brief Prints out an warning message
 * 
 * @param message 
 */
DebuggerPrinter::warning( std::string &message ) {
	printString(message, LogLevel::WARNING);
}