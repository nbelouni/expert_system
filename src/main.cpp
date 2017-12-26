#include "expertSystem.hpp"

int main(int argc, char **argv)
{
	try
	{
		if (argc < 2)
			std::cerr << "No file." << std::endl;
		else if (argc > 2)
			std::cerr << "Too many arguments." << std::endl;
		else
		{
			LexerParser tmp;
			tmp.Lexer(argv[1]);
		}
	}
	catch(std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}
