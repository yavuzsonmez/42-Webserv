#include "../../inc/DebuggerPrinter.hpp"
#include "../../inc/ServerBlock.hpp"

ServerBlock::ServerBlock()
{
    DebuggerPrinter debugger = debugger.getInstance();
}

ServerBlock::ServerBlock( const ServerBlock &src )
{

}

ServerBlock::~ServerBlock()
{

}

ServerBlock & ServerBlock::operator = (const ServerBlock &src) {
	return (*this);
}