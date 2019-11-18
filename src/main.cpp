#include "expertSystem.hpp"

int main(int argc, char **argv)
{
	if (argc < 2)
		std::cerr << "No file." << std::endl;
	else
	{
		for (int i = 1; i < argc; i++)
		{
			std::cout << "File : \"" << argv[i] << "\""<< std::endl << std::endl;
			LexerParser tmp;
			ExpertSystem *expertSystem = NULL;
			try
			{
				tmp.Lexer(argv[i]);

				expertSystem = tmp.Parser();
				expertSystem->printRules();
        		expertSystem->resolveAllQueries();
			}
			catch(std::exception &e)
			{
				std::cerr << e.what() << std::endl;
			}
			if (expertSystem)
				delete expertSystem;
			tmp.clear();
			std::cout << std::endl << std::endl;
		}
	}
	return 0;
}
