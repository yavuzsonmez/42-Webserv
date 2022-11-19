#ifndef DEBUGGER_PRINTER
# define DEBUGGER_PRINTER

#include "Singleton.hpp"
#include "../utility/utility.hpp"

#define USE_DEBUGGER DebuggerPrinter debugger = debugger.getInstance();

/**
 * @brief Loglevel of the enums
 * INFO i
 * WARNING W
 */
enum LogLevel {
	INFO = 'I',
	WARNING = 'W',
	DEBUG = 'D',
	ERROR = 'E'
};

/**
 * @brief	Simple debugger printer class.
 * 			Will only print out debug stuff during debug mode.
 * 			SINGLETON CLASS! DO NOT INITIALIZE. Use it like this:
 * 			DebuggerPrinter debugger = debugger.getInstance();
 * 			or (recommened):
 * 			use the macro USE_DEBUGGER
 * 			
 * 
 * @note	LEVEL INFO - INFO message
 * 			LEVEL WARNING - warning message
 * 			LEVEL DEBUG - DEBUG message
 * 			LEVEL ERROR - ERROR message ON STDERR
 */
class DebuggerPrinter: public Singleton<DebuggerPrinter> {
	public:
		DebuggerPrinter();
		DebuggerPrinter( const DebuggerPrinter &src );
		~DebuggerPrinter();
		DebuggerPrinter & operator = (const DebuggerPrinter &src);
		void info( const std::string &message );
		void warning( const std::string &message );
		void error( const std::string &message );
		void debug( const std::string &message );
	private:
		void printString( const std::string &message, LogLevel level );
};

#endif