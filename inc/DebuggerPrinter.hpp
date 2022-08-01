#include <string>
#include <iostream>
#include "Singleton.hpp"

/**
 * @brief Loglevel of the enums
 * INFO i
 * WARNING W
 */
enum LogLevel { 
	INFO = 'I', 
	WARNING = 'W',
};

/**
 * @brief	Simple debugger printer class.
 * 			Will only print out debug stuff during debug mode.
 * 
 * @note	LEVEL INFO - info message
 * 			LEVEL WARNING - warning message
 */
class DebuggerPrinter: public Singleton {
	public:
		DebuggerPrinter();
		DebuggerPrinter( const DebuggerPrinter &src );
		~DebuggerPrinter();
		DebuggerPrinter & operator = (const DebuggerPrinter &src);
		void info( std::string &message );
		void warning( std::string &message );
	private:
		void printString( std::string &message, LogLevel level );
};