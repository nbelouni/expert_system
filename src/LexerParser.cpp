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
		: 							"QUERY"
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
		: 							"?"
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
					std::cout << ". " << static_cast<int>(*i)  << ", " << *i << std::endl;

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
						std::cout << lexem_length  << ": " << _lexem[j] << std:: endl;
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

ExpertSystem	LexerParser::Parser()
{
	ExpertSystem expertSystem = ExpertSystem();

	Rule newRule = Rule();
    
	t_vector::iterator i = _lexedFile.begin();

	int nLines = 1;
	while (i != _lexedFile.end())
	{
		if ((i + 1) == _lexedFile.end() &&
			i->second != OPERAND && i->second != ENDL)
			throw InvalidLineException("Unexpected token : line " + std::to_string(nLines) + " : la " + printLexemValue((i + 1)->second));
		else if (i->second == O_BRACKET)
		{
			if ((i + 1)->second != NEGATIVE && (i + 1)->second != OPERAND)
				throw InvalidLineException("Unexpected token : line " + std::to_string(nLines) + " : " + printLexemValue((i + 1)->second));
		}
		else if (i->second == C_BRACKET)
		{
			if ((i + 1)->second == NEGATIVE || (i + 1)->second == OPERAND || (i + 1)->second == O_BRACKET)
				throw InvalidLineException("Unexpected token : line " + std::to_string(nLines) + " : " + printLexemValue((i + 1)->second));
		}
		else if (i->second == NEGATIVE)
		{
			if ((i + 1)->second != O_BRACKET && (i + 1)->second != OPERAND)
				throw InvalidLineException("Unexpected token : line " + std::to_string(nLines) + " : " + printLexemValue((i + 1)->second));
		}
		else if (i->second == AND || i->second == OR || i->second == XOR)
		{
			if ((i + 1)->second != O_BRACKET && (i + 1)->second != OPERAND && (i + 1)->second != NEGATIVE)
				throw InvalidLineException("Unexpected token : line " + std::to_string(nLines) + " : " + printLexemValue((i + 1)->second));
		}
		else if (i->second == DOUBLE_IMPLIES || i->second == IMPLIES)
		{
			if ((i + 1)->second != O_BRACKET && (i + 1)->second != OPERAND && (i + 1)->second != NEGATIVE)
				throw InvalidLineException("Unexpected token : line " + std::to_string(nLines) + " : " + printLexemValue((i + 1)->second));
		}
		else if (i->second == OPERAND)
		{
			if ((i + 1) != _lexedFile.end() &&
				((i + 1)->second == NEGATIVE || (i + 1)->second == O_BRACKET))
				throw InvalidLineException("Unexpected token : line " + std::to_string(nLines) + " : " + printLexemValue((i + 1)->second));
		}
		else if (i->second == FACTS)
		{
			if ((i + 1)->second != OPERAND && (i + 1)->second != QUERY)
				throw InvalidLineException("Unexpected token : line " + std::to_string(nLines) + " : " + printLexemValue((i + 1)->second));
		}
		else if (i->second == QUERY)
		{
			if ((i + 1)->second != OPERAND)
				throw InvalidLineException("Unexpected token : line " + std::to_string(nLines) + " : " + printLexemValue((i + 1)->second));
		}
		else if (i->second == ENDL)
			nLines++;
		i++;
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
