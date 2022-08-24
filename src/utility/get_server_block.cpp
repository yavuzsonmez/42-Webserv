#include "../../inc/utility/utility.hpp"

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
 * @param position  of the line to skip to or 0 if the end was reached
 *                  if -1 is being returned, an error occurred.
 *
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
		if (line.find("server {") == 0)
		{
			if (line == "server {")
				return i;
			return -2;
		}
		i++;
	};
	return 0;
}

/**
 * Returns the index where the first bracket of the string gets closed.
 * @returns the index of the closing bracket
 */
int findClosingBracket(std::string file_content) {
	std::stack <char>	bracket;
	char	c;

	for (size_t i = 0; i < file_content.length(); i++)
	{
		c = file_content[i];
		switch (c) // keep iterating over every line
		{
			case '{':
				bracket.push(c);
				break;
			case '}':
				if (bracket.empty() || bracket.top()!='{')
					return -1;
				else bracket.pop();
					if (bracket.empty())
						return i;
					break;
			default: ;
		}
	}
	return -1;
}

/**
 * Returns the server blocks as strings.
 * Each block returned represents a server block.
 *
 * @param file_content
 * @return server_blocks as string vector
 *
 * Will read in file line by line.
 * If a server_block is being found, it sets the bool in_server_block = true;
 * Then we keep iterating until the server block is being closed.
 *
 */
std::vector<std::string> get_server_blocks(std::string &file_content)
{
	std::vector<std::string> servers;
	bool in_server_block = false;

	std::istringstream iss(file_content);
	std::string result;
	int i = 0;
	for (std::string line; std::getline(iss, line); )
	{
		// Check if server block closes
		if (line.find("{") == 0) {

		}
		// Check for server {
		if (line.find("server {") == 0)
		{
			if (line == "server {")
				in_server_block = true;
			else
				throw("Invalid configuration file!");
		}
	};

	return servers;
}



// int main() {
// 	int line2 = getNextServerPrefix("Linne Line Line Line Linne\nserver {\niyasgdfoiugyasgyu\n\nserver {\negtriryg87w4eo57r", 0);
// 	std::cout << line2 << std::endl;
// 	int line3 = getNextServerPrefix("Linne Line Line Line Linne\nserver {\niyasgdfoiugyasgyu\n\nserver {\negtriryg87w4eo57rLinne Line Line Line Linne\nserver {\niyasgdfoiugyasgyu\n\nserver {\negtriryg87w4eo57r", line2);
// 	std::cout << line3 << std::endl;
//  }