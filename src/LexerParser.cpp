#include "LexerParser.hpp"

LexerParser::LexerParser(void)
{
	_lexem.push_back("#");
	_lexem.push_back("(");
	_lexem.push_back(")");
	_lexem.push_back("!");
	_lexem.push_back("+");
	_lexem.push_back("|");
	_lexem.push_back("^");
	_lexem.push_back("<=>");
	_lexem.push_back("=>");
	_lexem.push_back("[A-Z]");
	_lexem.push_back("=");
	_lexem.push_back("?");
}

LexerParser::LexerParser(LexerParser const &lp)
{
	*this = lp;
}

LexerParser::~LexerParser(void)
{
	clear();
}

LexerParser		&LexerParser::operator=(LexerParser const &rhs)
{
	_lexedFile = rhs.getLexedFile();
	return *this;
}

void		LexerParser::clear(void)
{
	_lexedFile.clear();
	_lexem.clear();
}

/*
 *	Get/Set
 */

LexerParser::t_vector	const &LexerParser::getLexedFile(void) const
{
	return _lexedFile;
}

/*
 *	Lexer / Parser
 */

std::string			printLexem(t_lexem lex)
{
	return std::string(
		  (lex == COMMENT) ?		"COMMENT"
		: (lex == O_BRACKET) ?		"O_BRACKET"
		: (lex == C_BRACKET) ?		"C_BRACKET"
		: (lex == NEGATIVE) ?		"NEGATIVE"
		: (lex == AND) ?			"AND"
		: (lex == OR) ?				"OR"
		: (lex == XOR) ?			"XOR"
		: (lex == DOUBLE_IMPLIES) ?	"DOUBLE_IMPLIES"
		: (lex == IMPLIES) ? 		"IMPLIES"
		: (lex == OPERAND) ?		"OPERAND"
		: (lex == FACTS) ?			"FACTS"
		: (lex == QUERY) ?			"QUERY"
		:							"ENDL"
	);
}

std::string			printLexemValue(t_lexem lex)
{
	return std::string(
		  (lex == COMMENT) ?		"#"
		: (lex == O_BRACKET) ?		"("
		: (lex == C_BRACKET) ?		")"
		: (lex == NEGATIVE) ?		"!"
		: (lex == AND) ?			"+"
		: (lex == OR) ?				"|"
		: (lex == XOR) ?			"^"
		: (lex == DOUBLE_IMPLIES) ?	"<=>"
		: (lex == IMPLIES) ? 		"=>"
		: (lex == OPERAND) ?		"[A-Z]"
		: (lex == FACTS) ?			"="
		: (lex == QUERY) ?			"?"
		: 							"end of line"
	);
}

void		LexerParser::printLexedFile()
{
	t_vector::iterator i = _lexedFile.begin();

	while (i != _lexedFile.end())
	{
		if (i->second == ENDL)
			std::cout << std::endl;
		else
		{
			std::cout <<  printLexem(i->second);
			std::cout << "(" << i->first << ") ";
		}
		i++;
	}
}

void		LexerParser::Lexer(char const *fileName)
{
	std::ifstream	file(fileName);
	std::string		line;

	if (file.is_open())
	{
		size_t line_index = 0;
		while (std::getline(file, line))
		{
			line = std::regex_replace(line, std::regex("([ ]+)"), "");

			std::string::iterator i = line.begin();

			while (i != line.end())
			{
				int j = 0;

				if (!(std::strncmp(&*i, _lexem[COMMENT].c_str(), 1)) || *i == 0)
					break;
				for (j = 1; j < 12; j++)
				{

					int lexem_length = (j == OPERAND) ? 1 : _lexem[j].length();

					if (!std::strncmp(&*i, _lexem[j].c_str(), lexem_length) ||
					(j == OPERAND && *i >= 'A' && *i <= 'Z'))
					{
						char str[lexem_length + 1];
						std::strncpy(str, &*i, lexem_length);	
						str[lexem_length] = 0;
						std::string _name(str);

						std::pair<std::string, t_lexem>	tmp(str, static_cast<t_lexem>(j));

						_lexedFile.push_back(tmp);
						i += lexem_length;
						break;
					}
				}
				if (j == 12)
				{
					throw InvalidLineException("Syntax error: line " + std::to_string(line_index) + ": \"" + *i + "\" unknown.");
				}

			}
			std::pair<std::string, e_lexem>	tmp("", ENDL);
			_lexedFile.push_back(tmp);
			line_index++;
		}
		file.close();
		printLexedFile();
	}
	else
		std::cerr << "Invalid file." << std::endl;
	
}

t_lexem			LexerParser::findNextLexem(t_vector::iterator i)
{
	while (i != _lexedFile.end() && i->second == ENDL)
		i++;
	if (i != _lexedFile.end())
		return i->second;
	return ENDL;
}

ExpertSystem	LexerParser::Parser()
{
	ExpertSystem expertSystem = ExpertSystem();

	Rule newRule = Rule();

	std::vector<Token> newTokenList;
    
	t_vector::iterator i = _lexedFile.begin();
	int nLines = 1;
	t_lexem nextLexem;

	while (i != _lexedFile.end())
	{
		if (i->second != OPERAND && i->second != ENDL 
			&& (i + 1) != _lexedFile.end() && findNextLexem(i + 1) == ENDL)
		{
			throw InvalidLineException("Unexpected token : line " + std::to_string(nLines) + " : " + i->first);
		}

		nextLexem = findNextLexem(i + 1);
		std::string error = "Unexpected token : line " + std::to_string(nLines) + " : " + i->first + " " + printLexemValue(nextLexem);

		if (i->second == ENDL)
		{
			nLines++;
		}

		else if (i->second == O_BRACKET)
		{
			if (nextLexem != NEGATIVE && nextLexem != OPERAND)
				throw InvalidLineException(error);
			newTokenList.push_back(Token(i->second, NULL, NULL, false));
		}
		else if (i->second == NEGATIVE)
		{
			std::cout << "ON FAIT QUOI POUR CA ? " << std::endl;
			if (nextLexem != O_BRACKET && nextLexem != OPERAND)
				throw InvalidLineException(error);
			newTokenList.push_back(Token(i->second, NULL, NULL, false));
		}
		else if (i->second == C_BRACKET)
		{
			newTokenList.push_back(Token(i->second, NULL, NULL, false));
			if (nextLexem == NEGATIVE || nextLexem == OPERAND || nextLexem == O_BRACKET)
			{
				newRule.setConsequents(newTokenList);
				newTokenList.clear();
				expertSystem.addRule(newRule);
				newRule.clear();
			}
		}
		else if (i->second == DOUBLE_IMPLIES || i->second == IMPLIES)
		{
			if (nextLexem != O_BRACKET && nextLexem != OPERAND && nextLexem != NEGATIVE)
				throw InvalidLineException(error);
			/*
			**	if first element
			*/
//			if (newTokenList.size() == 0)
//				throw InvalidLineException(error);

			newRule.setAntecedents(newTokenList);
			newTokenList.clear();
		}
		else if (i->second == AND || i->second == OR || i->second == XOR)
		{
			std::cout << "AJOUTER FONCTION" << std::endl;
			if (nextLexem != O_BRACKET && nextLexem != OPERAND && nextLexem != NEGATIVE)
				throw InvalidLineException(error);
			newTokenList.push_back(Token(i->second, NULL, NULL, false));
		}
		else if (i->second == FACTS)
		{
			if (nextLexem != OPERAND && nextLexem != QUERY)
				throw InvalidLineException(error);
			std::cout << "SET FACTS ";
		}
		else if (i->second == QUERY)
		{
			if (nextLexem != OPERAND)
				throw InvalidLineException(error);
			std::cout << "SET QUERIES ";
		}


		else if (i->second == OPERAND)
		{
			std::cout << "OPERAND" << std::endl;
			bool sign = true;
			if (i - 1 >= _lexedFile.begin() && (i - 1)->second == NEGATIVE)
				sign = false;

			if (!expertSystem.findOperand(i->first.c_str()[0]))
				expertSystem.addOperand(Operand(i->first.c_str()[0]));
			newTokenList.push_back(Token(i->second, expertSystem.getOperand(i->first.c_str()[0]), NULL, sign));

			if ((i + 1) != _lexedFile.end() &&
				(nextLexem == NEGATIVE || nextLexem == O_BRACKET || nextLexem == OPERAND))
			{
//			std::cout << printLexem(i->second) << " -> " << printLexem(nextLexem) << std::endl;

				if (newRule.getAllConsequents().size() == 0)
					newRule.setConsequents(newTokenList);
				else
				{
					newRule.setAntecedents(newTokenList);
					expertSystem.addRule(newRule);
					newRule.clear();
				}
				newTokenList.clear();
			}
		}
		if (i->second != ENDL)
			std::cout << "REMPLIR ICI " << printLexem(i->second) << std::endl;
		std::cout << "newTokenList size : " << newTokenList.size() << std::endl;
		std::cout << "Rule antecedents size : " << newRule.getAllAntecedents().size() << std::endl;
		std::cout << "Rule consequents size : " << newRule.getAllConsequents().size() << std::endl;
		i++;
	}
	std::cout << expertSystem.getAllRules().size() << std::endl;
	for (size_t i = 0; i < expertSystem.getAllRules().size(); i++)
	{
		for (size_t j = 0; i < expertSystem.getAllRules()[i].getAllAntecedents().size(); i++)
		{
			std::cout << printLexem(expertSystem.getAllRules()[i].getAllAntecedents()[j].getType()) ;
			std::cout << " ";
			if (expertSystem.getAllRules()[i].getAllAntecedents()[j].getType() == OPERAND)
				std::cout << printLexemValue(expertSystem.getAllRules()[i].getAllAntecedents()[j].getOperand().getName()) << std::endl;
		}
	}
	return ExpertSystem();
}

/*
 *	Nested Classes
 */

LexerParser::InvalidLineException::InvalidLineException(std::string s) : _message(s)
{
}

LexerParser::InvalidLineException::InvalidLineException(InvalidLineException const &i)
{
	*this = i;
}

LexerParser::InvalidLineException::~InvalidLineException() throw()
{
	_message.clear();
}

LexerParser::InvalidLineException &LexerParser::InvalidLineException::operator=(InvalidLineException const &d)
{
	(void)d;
	return *this;
}

char const	*LexerParser::InvalidLineException::what() const throw()
{
	return (_message.c_str());
}
