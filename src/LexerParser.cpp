#include "LexerParser.hpp"

LexerParser::LexerParser(void)
{
	_facts = false;
	_queries = false;
	_brackets = 0;
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
					throw InvalidLineException("Syntax error: line " + std::to_string(line_index) + ": \"" + *i + "\" unknown.");

			}
			std::pair<std::string, e_lexem>	tmp("", ENDL);
			_lexedFile.push_back(tmp);
			line_index++;
		}
		file.close();
	}
	else
		std::cerr << "Invalid file." << std::endl;
}

void			printTokenList(std::vector<Token> newTokenList)
{
		std::cout << "____________________ newTokenList  _____________________ : " << newTokenList.size() << std::endl;
		for (size_t j = 0; j < newTokenList.size(); j++)
		{
			std::cout << printLexem(newTokenList[j].getType()) << " " ;
			if (newTokenList[j].getType() == OPERAND)
				std::cout << newTokenList[j].getOperand()->getName() << " ";
		}
		std::cout << std::endl;
		std::cout << "____________________ _____________________" <<  std::endl;
}

t_lexem			LexerParser::findNextLexem(t_vector::iterator i)
{
	while (i != _lexedFile.end() && i->second == ENDL)
		i++;
	if (i != _lexedFile.end())
		return i->second;
	return ENDL;
}

void			LexerParser::addOBracket(t_vector::iterator i, std::vector<Token> &newTokenList, t_lexem nextLexem)
{
	if (_facts == true || _queries == true)
		throw InvalidLineException(_factsAndQueriesError);
	else if (nextLexem != NEGATIVE && nextLexem != OPERAND)
		throw InvalidLineException(_error);

	newTokenList.push_back(Token(i->second, NULL, NULL, false));
	_brackets += 1;
}

void			LexerParser::addNegative(t_lexem nextLexem)
{
	if (_facts == true || _queries == true)
		throw InvalidLineException(_factsAndQueriesError);
	else if (nextLexem != O_BRACKET && nextLexem != OPERAND)
		throw InvalidLineException(_error);
}

void			LexerParser::addCBracket(t_vector::iterator i, std::vector<Token> &newTokenList, t_lexem nextLexem, Rule &newRule, ExpertSystem &expertSystem)
{
	if (_facts == true || _queries == true)
		throw InvalidLineException(_factsAndQueriesError);

	_brackets -= 1;
	if (_brackets < 0)
		throw InvalidLineException(_error + " : parse _error near ')'");

	newTokenList.push_back(Token(i->second, NULL, NULL, false));

	if (nextLexem == NEGATIVE || nextLexem == OPERAND || nextLexem == O_BRACKET)
	{
		if (newRule.getAllAntecedents().empty())
			throw InvalidLineException(_error);

		newRule.setConsequents(newTokenList);
		newTokenList.clear();
		expertSystem.addRule(newRule);
		newRule.clear();
	}
}

void			LexerParser::addImplies(std::vector<Token> &newTokenList, t_lexem nextLexem, Rule &newRule, int nLines)
{
	if (_facts == true || _queries == true)
		throw InvalidLineException(_factsAndQueriesError);
	else if ((nextLexem != O_BRACKET && nextLexem != OPERAND && nextLexem != NEGATIVE) || newTokenList.size() == 0)
		throw InvalidLineException(_error);
	else if (!newRule.getAllAntecedents().empty())
		throw InvalidLineException("Unexpected token : line " + std::to_string(nLines) + " : the expression must contain only one '=>'.");
	else if (_brackets > 0)
		throw InvalidLineException("Unexpected token : line " + std::to_string(nLines) + " : '(' not closed.");

	newRule.setAntecedents(newTokenList);
	newTokenList.clear();
}

void			LexerParser::addOperator(t_vector::iterator i, std::vector<Token> &newTokenList, t_lexem nextLexem)
{
	if (_facts == true || _queries == true)
		throw InvalidLineException(_factsAndQueriesError);
	else if (nextLexem != O_BRACKET && nextLexem != OPERAND && nextLexem != NEGATIVE)
		throw InvalidLineException(_error);

	newTokenList.push_back(Token(i->second, NULL, NULL, false));
}

void			LexerParser::addFacts(t_lexem nextLexem)
{
	if (nextLexem != OPERAND && nextLexem != QUERY)
		throw InvalidLineException(_error);
	_facts = true;
}

void			LexerParser::addQueries(t_lexem nextLexem)
{
	if (nextLexem != OPERAND || _facts == false)
		throw InvalidLineException(_error);
	_queries = true;
}

void			LexerParser::addOperand(t_vector::iterator i, std::vector<Token> &newTokenList, t_lexem nextLexem, Rule &newRule, ExpertSystem &expertSystem, int nLines)
{
	bool sign = true;
	if (i - 1 >= _lexedFile.begin() && (i - 1)->second == NEGATIVE)
		sign = false;

	if (_facts == false && _queries == false)
	{
		if (!expertSystem.findOperand(i->first.c_str()[0]))
			expertSystem.addOperand(new Operand(i->first.c_str()[0]));

		newTokenList.push_back(Token(i->second, expertSystem.findOperand(i->first.c_str()[0]), NULL, sign));
        
		if ((i + 1) != _lexedFile.end() &&
			(nextLexem == NEGATIVE || nextLexem == O_BRACKET || nextLexem == OPERAND || nextLexem == FACTS))
		{
			if (_brackets > 0)
				throw InvalidLineException("Unexpected token : line " + std::to_string(nLines) + " : '(' not closed.");

			if (newRule.getAllAntecedents().size() == 0)
			{
				if (nextLexem != IMPLIES && nextLexem != DOUBLE_IMPLIES)
					throw InvalidLineException(_error);
				newRule.setAntecedents(newTokenList);
			}
			else
			{
				newRule.setConsequents(newTokenList);
				expertSystem.addRule(newRule);
				newRule.clear();
			}
			newTokenList.clear();
		}
	}
	else if (_facts == true && _queries == false)
	{
		if (!expertSystem.findOperand(i->first.c_str()[0]))
			throw InvalidLineException("Unexpected token : line " + std::to_string(nLines) + " : " + i->first + " does not exist.");
		expertSystem.findOperand(i->first.c_str()[0])->setValue(TRUE);
		expertSystem.findOperand(i->first.c_str()[0])->setIsResolved(TRUE);
	}
	else if (_facts == true && _queries == true)
	{
		if (!expertSystem.findOperand(i->first.c_str()[0]))
			throw InvalidLineException("Unexpected token : line " + std::to_string(nLines) + " : " + i->first + " does not exist.");
		expertSystem.pushQuery(expertSystem.findOperand(i->first.c_str()[0]));
	}
	else
		throw InvalidLineException(_error);

}

ExpertSystem	LexerParser::Parser()
{
	ExpertSystem		expertSystem = ExpertSystem();
	Rule				newRule = Rule();
	std::vector<Token>	newTokenList;
	t_lexem 			nextLexem;

	int 				nLines = 1;

	t_vector::iterator	i = _lexedFile.begin();

	try
	{
		while (i != _lexedFile.end())
		{
			if (i->second != OPERAND && i->second != ENDL 
				&& (i + 1) != _lexedFile.end() && findNextLexem(i + 1) == ENDL)
				throw InvalidLineException("Unexpected token : line " + std::to_string(nLines) + " : " + i->first);
            
			nextLexem = findNextLexem(i + 1);
            
			_error = "Unexpected token : line " + std::to_string(nLines) + " : " + i->first + " " + printLexemValue(nextLexem);
			_factsAndQueriesError = "Unexpected token : line " + std::to_string(nLines) + " : facts and queries can only contain letters.";
            
			if (i->second == ENDL)
				nLines++;
			else if (i->second == O_BRACKET)
				addOBracket(i, newTokenList, nextLexem);
			else if (i->second == NEGATIVE)
				addNegative(nextLexem);
			else if (i->second == C_BRACKET)
				addCBracket(i, newTokenList, nextLexem, newRule, expertSystem);
			else if (i->second == DOUBLE_IMPLIES || i->second == IMPLIES)
				addImplies(newTokenList, nextLexem, newRule, nLines);
			else if (i->second == AND || i->second == OR || i->second == XOR)
				addOperator(i, newTokenList, nextLexem);
			else if (i->second == FACTS)
				addFacts(nextLexem);
			else if (i->second == QUERY)
				addQueries(nextLexem);
			else if (i->second == OPERAND)
				addOperand(i, newTokenList, nextLexem, newRule, expertSystem, nLines);
			i++;
		}

		if (expertSystem.getAllRules().empty() || !_facts || !_queries)
			throw InvalidLineException("Missing elements.");

	}
	catch(std::exception &e)
	{
		throw e;
	}

	expertSystem.printRules();
	expertSystem.printOperands();
	std::cout << "ok" << std::endl;
	return expertSystem;
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
