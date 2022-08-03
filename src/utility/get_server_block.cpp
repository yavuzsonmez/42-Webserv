#include "../../inc/utility.hpp"
#include <vector>
#include <string>

/**
 * Jumps to the next server prefix to get the server block.
 * A server block always starts with the line
 * server {
 * and nothing else will be accepted as a prefix for a serverblock.
 * 
 * Returns the line of the start of the next server block.
 * Takes in the last read line and skips all until it hits line positon
 * 
 * @param filecontent of the fiel
 * @param position of the line to skip to or -1 if the end was reached
 * 
 * @note We start counting at 0
 */
int getNextServerPrefix(std::string file_content, int position) {
    std::istringstream iss(file_content);
	std::string result;
    int i = 0;
	for (std::string line; std::getline(iss, line); )
	{
        // skip all characters before position
        if (position >= i)
        {
            i++; 
            continue;
        }
        // Check for server {
        if (line == "server {")
            return i;
        i++;
	};
    return -1;
}

/**
 * Counts how many server blocks are in the configuration file
 */
int getCountOfServerBlocks(std::string file_content) {

}

/**
 * Returns the server blocks as strings.
 * Each block returned represents a server block.
 * @param file_content
 * @return server_blocks as string vector
 */
std::vector<std::string> get_server_blocks(std::string &file_content)
{
    std::vector<std::string> servers;


    return servers;
}



// int main() {
//     int line = getNextServerPrefix("Linne Line Line Line Linne\nserver {\niyasgdfoiugyasgyu\n\nserver {\negtriryg87w4eo57r", 0);
//     std::cout << line << std::endl;
//     int line2 = getNextServerPrefix("Linne Line Line Line Linne\nserver {\niyasgdfoiugyasgyu\n\nserver {\negtriryg87w4eo57r", line);
//     std::cout << line2 << std::endl;
//     int line3 = getNextServerPrefix("Linne Line Line Line Linne\nserver {\niyasgdfoiugyasgyu\n\nserver {\negtriryg87w4eo57rLinne Line Line Line Linne\nserver {\niyasgdfoiugyasgyu\n\nserver {\negtriryg87w4eo57r", line2);
//     std::cout << line3 << std::endl;
// }