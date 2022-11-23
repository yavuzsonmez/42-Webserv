#include "../../inc/debugger/DebuggerPrinter.hpp"
#include <signal.h>

/**
 * @brief Send a kill signal to the process with the given pid
 * 
 * @param pid 
 * @param sig 
 * @return int 
 */
int kill_with_error(int pid)
{
	USE_DEBUGGER;
	int ret = kill(pid, SIGKILL);
	if (ret == -1)
	{
		debugger.error("kill_with_error() failed");
		debugger.error("kill() failed" + std::string(strerror(errno)));
		return -1;
	}
	return ret;
}