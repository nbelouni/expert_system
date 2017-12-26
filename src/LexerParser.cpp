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

void		LexerParser::printLexedFile()
{
	t_vector::iterator i = _lexedFile.begin();

	while (i != _lexedFile.end())
	{
		if (i->second == ENDL)
			std::cout << std::endl;
		else
		{
			std::cout << 
			(
				  (i->second == COMMENT) ?			"COMMENT"
				: (i->second == O_BRACKET) ?		"O_BRACKET"
				: (i->second == C_BRACKET) ?		"C_BRACKET"
				: (i->second == NEGATIVE) ?			"NEGATIVE"
				: (i->second == AND) ?				"AND"
				: (i->second == OR) ?				"OR"
				: (i->second == XOR) ?				"XOR"
				: (i->second == DOUBLE_IMPLIES) ?	"DOUBLE_IMPLIES"
				: (i->second == IMPLIES) ? 			"IMPLIES"
				: (i->second == OPERAND) ?			"OPERAND"
				: (i->second == FACTS) ?			"FACTS"
				: 									"QUERY"
			);
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
					throw InvalidLineException("Syntax error.");
				}

			}
			std::pair<std::string, e_lexem>	tmp("", ENDL);
			_lexedFile.push_back(tmp);
		}
		file.close();
		printLexedFile();
	}
	else
		std::cerr << "Invalid file." << std::endl;
	
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
