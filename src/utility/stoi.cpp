#include "../../inc/utility/utility.hpp"

/**
 * @brief Stoi replacement for c11
 * 
 * @param s 
 * @return int 
 */
int stoi_replacement( std::string s )
{
    int i;
    std::istringstream(s) >> i;
    return i;
}