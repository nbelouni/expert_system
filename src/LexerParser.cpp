#include "LexerParser.hpp"

LexerParser::LexerParser(void)
{
	_facts = false;
	_queries = false;
	_brackets = 0;
	_lexem.push_back(t_ash_lexem{"#", COMMENT, "COMMENT", nullptr});
	_lexem.push_back(t_ash_lexem{"!", NEGATIVE, "NEGATIVE", &LexerParser::addNegative});
	_lexem.push_back(t_ash_lexem{"+", AND, "AND", &LexerParser::addOperator});
	_lexem.push_back(t_ash_lexem{"|", OR, "OR", &LexerParser::addOperator});
	_lexem.push_back(t_ash_lexem{"^", XOR, "XOR", &LexerParser::addOperator});
	_lexem.push_back(t_ash_lexem{"<=>", DOUBLE_IMPLIES, "DOUBLE_IMPLIES", &LexerParser::addImplies});
	_lexem.push_back(t_ash_lexem{"=>", IMPLIES, "IMPLIES", &LexerParser::addImplies});
	_lexem.push_back(t_ash_lexem{"[A-Z]", OPERAND, "OPERAND", &LexerParser::addOperand});
	_lexem.push_back(t_ash_lexem{"=", FACTS, "FACTS", &LexerParser::addFacts});
	_lexem.push_back(t_ash_lexem{"?", QUERY, "QUERY", &LexerParser::addQueries});
	_lexem.push_back(t_ash_lexem{"(", O_BRACKET, "O_BRACKET", &LexerParser::addOBracket});
	_lexem.push_back(t_ash_lexem{")", C_BRACKET, "C_BRACKET", &LexerParser::addCBracket});
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
	_exception.clear();
}

/*
 *	Get/Set
 */

LexerParser::t_vector	const &LexerParser::getLexedFile(void) const
{
	return _lexedFile;
}

void 						LexerParser::addExceptionMessage(std::string newMessage)
{
	_exception.push_back(newMessage);
}

bool						LexerParser::exceptionEmpty(void)
{
	if (!_exception.empty())
	{
		throw InvalidLineException(_exception);
	}
	return false;
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
	try
	{
		int fd;
		if ((fd = open(fileName, O_RDONLY)) < 0)
		{
			std::cerr << "\"" << fileName << "\" is invalid." << std::endl;
			return;
		}
		else
			close(fd);

		std::ifstream	file(fileName);
		std::string		line;

		if (file && file.is_open())
		{
			int line_index = 0;
			size_t i = 0;
			int j = 0;
			int lexem_length;
			while (std::getline(file, line))
			{
				line = std::regex_replace(line, std::regex("([	 ]+)"), "");

				i = 0;
				while (i < line.size())
				{
					if (line[i] == *_lexem[COMMENT].value.c_str() || line[i] == 0)
						break;
					for (j = 1; j < 12; j++)
					{
						lexem_length = (j == OPERAND) ? 1 : _lexem[j].value.length();
		
						if (i + lexem_length <= line.size() && (!std::strncmp(line.c_str() + i, _lexem[j].value.c_str(), lexem_length) || (j == OPERAND && line[i] >= 'A' && line[i] <= 'Z')))
						{
							std::string str(line.c_str() + i, 0, lexem_length);
							std::pair<std::string, t_lexem>	tmp(str, static_cast<t_lexem>(j));
							_lexedFile.push_back(tmp);
							i += lexem_length;

							break;
						}
					}
					if (j == 12)
					{
						if (i <= line.size())
						{
							std::string error("Syntax error: line ");
							error.append(std::to_string(line_index));
							error.append(": ");
							error.append(&(line.c_str()[i]), 1);
							error.append(" unknown.");
							addExceptionMessage(error);
							error.clear();
						}
						i++;
					}
				}
				std::pair<std::string, e_lexem>	tmp("", ENDL);
				_lexedFile.push_back(tmp);
				line_index++;
			}
			file.close();
		}
		exceptionEmpty();
	}
	catch(std::exception &e)
	{
		std::rethrow_exception(std::current_exception());
	}
}

void			printToken(Token &t)
{
	if (t.getIsNegativeOperand() == true)
		std::cout << "!";
	if (t.getType() == OPERAND)
		std::cout << t.getOperand()->getName() << " ";
	else
	std::cout << printLexemValue(t.getType()) << " " ;
}

void			printTokenList(std::vector<Token> newTokenList, bool endl)
{
		for (size_t j = 0; j < newTokenList.size(); j++)
		{
			printToken(newTokenList[j]);
		}
		if (endl)
			std::cout << std::endl;
}

std::string		tokenListToString(std::vector<Token> newTokenList, bool endl)
{
	std::string	out;

	for (size_t j = 0; j < newTokenList.size(); j++)
	{
		if (newTokenList[j].getIsNegativeOperand() == true)
			out.append("!");
		if (newTokenList[j].getType() == OPERAND)
		{
			out.push_back(newTokenList[j].getOperand()->getName());
			out.append(" ");
		}
		else
			out.append(printLexemValue(newTokenList[j].getType()) + " ");
	}
	if (endl)
		out.append("\n");

	return out;
}

std::string		ruleToString(Rule r)
{
	std::string	out;
	out.append(tokenListToString(r.getAllAntecedents(), false));
	
	out.append(printLexemValue(r.getImplying()) + " ");
	out.append(tokenListToString(r.getAllConsequents(), true));

	return out;
}

t_lexem			LexerParser::findNextLexem(t_vector::iterator i)
{
	while (i != _lexedFile.end() && i->second == ENDL)
		i++;
	if (i != _lexedFile.end())
		return i->second;
	return ENDL;
}

/*
**	All the functions addXxx(i, args) are in _lexem (hashtable) (initialized line 6)
**
**	They are called in Parser()
**
**	All exceptions are kept until the end of parsing
*/

void			LexerParser::addOBracket(t_vector::iterator i, std::vector<void *> args)
{
	if (_facts == true || _queries == true)
		addExceptionMessage(_factsAndQueriesError);
	else if (T_LEXEM(args[1]) != O_BRACKET && T_LEXEM(args[1]) != NEGATIVE && T_LEXEM(args[1]) != OPERAND)
	{
		addExceptionMessage(_error);
	}
	else
	{
		TOKEN_VECTOR(args[0])->push_back(Token(i->second, NULL, NULL, false));
		_brackets += 1;
	}
}

void			LexerParser::addNegative(t_vector::iterator i, std::vector<void *> args)
{
	static_cast<void>(i);
	if (_facts == true || _queries == true)
		addExceptionMessage(_factsAndQueriesError);
	else if (T_LEXEM(args[1]) != O_BRACKET && T_LEXEM(args[1]) != OPERAND)
		addExceptionMessage(_error);
}

void			LexerParser::addCBracket(t_vector::iterator i, std::vector<void *> args)
{
	if (_facts == true || _queries == true)
		addExceptionMessage(_factsAndQueriesError);

	_brackets -= 1;
	if (_brackets < 0)
		addExceptionMessage(_error + " : parse _error near \')\'");

	TOKEN_VECTOR(args[0])->push_back(Token(i->second, NULL, NULL, false));

	if (T_LEXEM(args[1]) == NEGATIVE || T_LEXEM(args[1]) == OPERAND || T_LEXEM(args[1]) == O_BRACKET)
	{
		if (RULE(args[2])->getAllAntecedents().empty())
			addExceptionMessage(_error);

		RULE(args[2])->setConsequents(*TOKEN_VECTOR(args[0]));
		TOKEN_VECTOR(args[0])->clear();
		EXPERT_SYSTEM(args[3])->addRule(*RULE(args[2]));
		RULE(args[2])->clear();
	}
}

void			LexerParser::addImplies(t_vector::iterator i, std::vector<void *> args)
{
	static_cast<void>(i);
	if (_brackets > 0)
	{
		addExceptionMessage("Unexpected token : line " + std::to_string(INT(args[4])) + " : \'(\' not closed.");
		return;
	}
	if (_facts == true || _queries == true)
		addExceptionMessage(_factsAndQueriesError);
	else if ((T_LEXEM(args[1]) != O_BRACKET && T_LEXEM(args[1]) != OPERAND && T_LEXEM(args[1]) != NEGATIVE) || TOKEN_VECTOR(args[0])->size() == 0)
		addExceptionMessage(_error);
	else if (!RULE(args[2])->getAllAntecedents().empty())
		addExceptionMessage("Unexpected token : line " + std::to_string(INT(args[4])) + " : the expression must contain only one \'=>\'.");
	else if (_brackets > 0)
		addExceptionMessage("Unexpected token : line " + std::to_string(INT(args[4])) + " : \'(\' not closed.");

	RULE(args[2])->setImplying(i->second);
	RULE(args[2])->setAntecedents(*TOKEN_VECTOR(args[0]));
	TOKEN_VECTOR(args[0])->clear();
}

void			LexerParser::addOperator(t_vector::iterator i, std::vector<void *> args)
{
	if (_facts == true || _queries == true)
		addExceptionMessage(_factsAndQueriesError);
	else if (T_LEXEM(args[1]) != O_BRACKET && T_LEXEM(args[1]) != OPERAND && T_LEXEM(args[1]) != NEGATIVE)
		addExceptionMessage(_error);
    switch (i->second) {
        case AND :
            TOKEN_VECTOR(args[0])->push_back(Token(i->second, NULL, &andOperator, false));
        break;
        case OR :
            TOKEN_VECTOR(args[0])->push_back(Token(i->second, NULL, &orOperator, false));
        break;
        case XOR :
            TOKEN_VECTOR(args[0])->push_back(Token(i->second, NULL, &xorOperator, false));
        break;
        default :
            std::cout << "ERROR" << std::endl;
        break;
    }
}

void			LexerParser::addFacts(t_vector::iterator i, std::vector<void *> args)
{
	static_cast<void>(i);

	if (T_LEXEM(args[1]) != OPERAND && T_LEXEM(args[1]) != QUERY)
		addExceptionMessage(_error);
	_facts = true;
}

void			LexerParser::addQueries(t_vector::iterator i, std::vector<void *> args)
{
	static_cast<void>(i);

	if (T_LEXEM(args[1]) != OPERAND || _facts == false)
		addExceptionMessage(_error);
	_queries = true;
}

void			LexerParser::addOperand(t_vector::iterator i, std::vector<void *> args)
{
	bool negSign = false;
	if (i - 1 >= _lexedFile.begin() && (i - 1)->second == NEGATIVE)
		negSign = true;

	//	operand in rules
	if (_facts == false && _queries == false)
	{
		//	creating unique operand
		if (!EXPERT_SYSTEM(args[3])->findOperand(i->first.c_str()[0]))
		{
			EXPERT_SYSTEM(args[3])->addOperand(new Operand(i->first.c_str()[0]));
		}

		//	adding operand to current rule
		TOKEN_VECTOR(args[0])->push_back(Token(i->second, EXPERT_SYSTEM(args[3])->findOperand(i->first.c_str()[0]), NULL, negSign));
        
		// if not end of file or is end of rule
		if ((i + 1) != _lexedFile.end() &&
			(T_LEXEM(args[1]) == NEGATIVE || T_LEXEM(args[1]) == O_BRACKET || T_LEXEM(args[1]) == OPERAND || T_LEXEM(args[1]) == FACTS))
		{
			//bracket not closed
			if (_brackets > 0)
			{
				addExceptionMessage("Unexpected token : line " + std::to_string(INT(args[4])) + " : \'(\' not closed.");
			}
			//	antecedents not set -> adding antecedent 
			// SHOULD NEVER HAPPEN BUT i dont remember why i did this, so i keep it
			else if (RULE(args[2])->getAllAntecedents().size() == 0)
			{
				if (T_LEXEM(args[1]) != IMPLIES && T_LEXEM(args[1]) != DOUBLE_IMPLIES)
					addExceptionMessage(_error);
				else
				{
					std::cout << "Setting antecedents 2" << std::endl;
					RULE(args[2])->setAntecedents(*TOKEN_VECTOR(args[0]));
					RULE(args[2])->setImplying(T_LEXEM(args[1]));
				}

			}
			else // antecedents already set -> set consequents
			{
				RULE(args[2])->setConsequents(*TOKEN_VECTOR(args[0]));
	//			RULE(args[2])->reorderTokenArrays();
				EXPERT_SYSTEM(args[3])->addRule(*RULE(args[2]));
				//if (RULE(args[2])->getContainsXor())
				//	std::cout << "RULE CONTAINS XOR !!" << std::endl;

				RULE(args[2])->clear(); 
				T_LEXEM(args[5]) = NONE;
			}
			TOKEN_VECTOR(args[0])->clear();
		}
	}
	else if (_facts == true && _queries == false)
	{
		if (!EXPERT_SYSTEM(args[3])->findOperand(i->first.c_str()[0]))
			addExceptionMessage("Unexpected token : line " + std::to_string(INT(args[4])) + " : " + i->first + " does not exist.");
		else
		{
			EXPERT_SYSTEM(args[3])->findOperand(i->first.c_str()[0])->setValue(TRUE);
			EXPERT_SYSTEM(args[3])->findOperand(i->first.c_str()[0])->setIsResolved(TRUE);
			EXPERT_SYSTEM(args[3])->findOperand(i->first.c_str()[0])->setInitialFact(TRUE);
		}
	}
	else if (_facts == true && _queries == true)
	{
		if (!EXPERT_SYSTEM(args[3])->findOperand(i->first.c_str()[0]))
			addExceptionMessage("Unexpected token : line " + std::to_string(INT(args[4])) + " : " + i->first + " does not exist.");
		else
			EXPERT_SYSTEM(args[3])->pushQuery(EXPERT_SYSTEM(args[3])->findOperand(i->first.c_str()[0]));
	}
	else
		addExceptionMessage(_error);

}

ExpertSystem	*LexerParser::Parser()
{
	ExpertSystem		*expertSystem = new ExpertSystem();
	Rule				newRule = Rule();
	std::vector<Token>	newTokenList;
	t_lexem 			nextLexem;

	int 				nLines = 1;
	t_lexem				lastImpliyng;
	t_vector::iterator	i = _lexedFile.begin();

	while (i != _lexedFile.end())
	{
		if (i->second != OPERAND && i->second != ENDL 
			&& (i + 1) != _lexedFile.end() && findNextLexem(i + 1) == ENDL)
			addExceptionMessage("Unexpected token : line " + std::to_string(nLines) + " : " + i->first);
        
		nextLexem = findNextLexem(i + 1);
       
		std::cout <<  i->first << std::endl;
		_error = "Unexpected token : line " + std::to_string(nLines) + " : " + i->first + " " + printLexemValue(nextLexem);
		_factsAndQueriesError = "Unexpected token : line " + std::to_string(nLines) + " : facts and queries can only contain letters.";
        
		std::vector<void *> args = std::vector<void *>{
            &newTokenList,
            &nextLexem,
            &newRule,
            &(*expertSystem),
            &nLines,
            &lastImpliyng
        };

		if (i->second == ENDL)
			nLines++;
		else
			(this->*_lexem[i->second].f)(i, args);
		i++;
		args.clear();
	}
	
	if (expertSystem->getAllRules().empty() || !_facts || !_queries)
		addExceptionMessage("Missing elements.");
	else

	try
	{
		exceptionEmpty();
	}
	catch(std::exception &e)
	{
		delete expertSystem;
		std::rethrow_exception(std::current_exception());
	}

	return expertSystem;
}

/*
 *	Nested Classes
 */

LexerParser::InvalidLineException::InvalidLineException(std::vector<std::string> errors)
{
	_messages = errors;
}

LexerParser::InvalidLineException::InvalidLineException(InvalidLineException const &i)
{
	*this = i;
}

LexerParser::InvalidLineException::~InvalidLineException() throw()
{
	_messages.clear();
}

LexerParser::InvalidLineException &LexerParser::InvalidLineException::operator=(InvalidLineException const &d)
{
	(void)d;
	return *this;
}

char const	*LexerParser::InvalidLineException::what() const throw()
{
	std::string ret;
	for (size_t i = 0; i < _messages.size(); i++)
	{
		ret.append(_messages[i] + "\n");
	}
	return (ret.c_str());
}

