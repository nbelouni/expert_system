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
			LexerParser lP;
			lP.Lexer(argv[1]);
			ExpertSystem expSys = lP.Parser();
			expSys.printRules();
			expSys.printOperands();

			Operand *tmp = expSys.popQuery();
			while (tmp)
			{
				t_status s = expSys.resolveQuery(tmp->getName());
				std::cout << "operand \"" << tmp->getName() << "\" : ";
				std::cout << (
					(s == TRUE) ?		"TRUE":
					(s == FALSE) ?		"FALSE":
					(s == UNDEFINED) ?	"UNDEFINED":
										"NOT RESOLVED"
				) ;
				std::cout << std::endl;
				tmp = expSys.popQuery();
			}

		}
	}
	catch(std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}
