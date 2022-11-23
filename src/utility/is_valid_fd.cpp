#include <fcntl.h>
#include <sys/errno.h>
#include "../../inc/utility/utility.hpp"
#include "../../inc/debugger/DebuggerPrinter.hpp"

/**
 * @brief Checks if a fd is valid. Return 1 if the fd is valid
 * @param fd 
 * @return int 
 */
int is_valid_fd(int fd)
{
	USE_DEBUGGER;
	return fcntl(fd, F_GETFL) != -1 || errno != EBADF;
}