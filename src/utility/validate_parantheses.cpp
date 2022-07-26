#include <string>
#include <stack>
#include <iostream>
//checks a string of parantheses for it's validity
bool	validate_parantheses(std::string parantheses)
{
	std::stack<char>	bracket;
	char	c;
	for (size_t i = 0; i < parantheses.length(); i++)
	{
		c = parantheses[i];
		switch (c)
		{
			case '(': bracket.push(c); break;
			case '{': bracket.push(c); break;
			case '[': bracket.push(c); break;
			case ')': if (bracket.empty() || bracket.top()!='(') return false; else bracket.pop(); break;
			case '}': if (bracket.empty() || bracket.top()!='{') return false; else bracket.pop(); break;
			case ']': if (bracket.empty() || bracket.top()!='[') return false; else bracket.pop(); break;
			default: ;
		}
	}
	return bracket.empty() ;
}
int	main()
{
	std::string parantheses = "[[]{}]()";
	bool check = validate_parantheses(parantheses);
	std::cout << "valid? " << check << std::endl;
	return (0);
}