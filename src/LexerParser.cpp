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
		throw InvalidLineException(_exception);
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

void		LexerParser::Lexer(char const *fileName)
{
	std::ifstream	file(fileName);

	if (file && file.is_open())
	{
		size_t          line_index = 0;
	    std::string		line;
        std::string     lexed_line;

		while (std::getline(file, line))
		{
			lexed_line = std::regex_replace(line, std::regex("([ ]+)"), "");

			std::string::iterator i = lexed_line.begin();

			while (i != lexed_line.end())
			{
				int j = 0;

				if (!(std::strncmp(&*i, _lexem[COMMENT].value.c_str(), 1)) || *i == 0)
					break;
				for (j = 1; j < 12; j++)
				{

					int lexem_length = (j == OPERAND) ? 1 : _lexem[j].value.length();

					if (!std::strncmp(&*i, _lexem[j].value.c_str(), lexem_length) ||
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
					addExceptionMessage("Syntax error: line " + std::to_string(line_index) + ": \"" + *i + "\" unknown.");
					i++;
				}
			}
            lexed_line.clear();
            line.clear();
			std::pair<std::string, e_lexem>	tmp("", ENDL);
			_lexedFile.push_back(tmp);
			line_index++;
		}
		file.close();
	}
	else
		addExceptionMessage("Invalid file.");
	try
	{
		exceptionEmpty();
	}
	catch(std::exception &e)
	{
		std::rethrow_exception(std::current_exception());
	}
}

void			printTokenList(std::vector<Token> newTokenList)
{
		for (size_t j = 0; j < newTokenList.size(); j++)
		{
			if (newTokenList[j].getIsNegativeOperand() == true)
				std::cout << "!";
			if (newTokenList[j].getType() == OPERAND)
				std::cout << newTokenList[j].getOperand()->getName() << " ";
			else
			std::cout << printLexemValue(newTokenList[j].getType()) << " " ;
		}
		std::cout << std::endl;
}

t_lexem			LexerParser::findNextLexem(t_vector::iterator i)
{
	while (i != _lexedFile.end() && i->second == ENDL)
		i++;
	if (i != _lexedFile.end())
		return i->second;
	return ENDL;
}

void			LexerParser::addOBracket(t_vector::iterator i, std::vector<void *> args) //[ [0] = std::vector<Token> &newTokenList, [1] = t_lexem nextLexem]
{
	if (_facts == true || _queries == true)
		addExceptionMessage(_factsAndQueriesError);
	else if (CAST_T_LEXEM(args[1]) != O_BRACKET && CAST_T_LEXEM(args[1]) != NEGATIVE && CAST_T_LEXEM(args[1]) != OPERAND)
		addExceptionMessage(_error);

	CAST_TOKEN_VECTOR(args[0])->push_back(Token(i->second, NULL, NULL, false));
	_brackets += 1;
}

void			LexerParser::addNegative(t_vector::iterator i, std::vector<void *> args)// [[0] = t_lexem nextLexem]
{
	static_cast<void>(i);
	if (_facts == true || _queries == true)
		addExceptionMessage(_factsAndQueriesError);
	else if (CAST_T_LEXEM(args[1]) != O_BRACKET && CAST_T_LEXEM(args[1]) != OPERAND)
		addExceptionMessage(_error);
}

void			LexerParser::addCBracket(t_vector::iterator i, std::vector<void *> args)//[[0] =  std::vector<Token> &newTokenList,[1] = t_lexem nextLexem,[2] = Rule &newRule,[3] = ExpertSystem &expertSystem]
{
	if (_facts == true || _queries == true)
		addExceptionMessage(_factsAndQueriesError);

	_brackets -= 1;
	if (_brackets < 0)
		addExceptionMessage(_error + " : parse _error near ')'");

	CAST_TOKEN_VECTOR(args[0])->push_back(Token(i->second, NULL, NULL, false));

	if (CAST_T_LEXEM(args[1]) == NEGATIVE || CAST_T_LEXEM(args[1]) == OPERAND || CAST_T_LEXEM(args[1]) == O_BRACKET)
	{
		if (CAST_RULE(args[2])->getAllAntecedents().empty())
			addExceptionMessage(_error);

		CAST_RULE(args[2])->setConsequents(*CAST_TOKEN_VECTOR(args[0]));
		CAST_TOKEN_VECTOR(args[0])->clear();
		CAST_EXPERT_SYSTEM(args[3])->addRule(*CAST_RULE(args[2]));
		CAST_RULE(args[2])->clear();
	}
}

void			LexerParser::addImplies(t_vector::iterator i, std::vector<void *> args)//[[0]std::vector<Token> &newTokenList,[1] t_lexem nextLexem,[2] Rule &newRule,[3] = 0 , [4] = int nLines]
{
	static_cast<void>(i);
	if (_facts == true || _queries == true)
		addExceptionMessage(_factsAndQueriesError);
	else if ((CAST_T_LEXEM(args[1]) != O_BRACKET && CAST_T_LEXEM(args[1]) != OPERAND && CAST_T_LEXEM(args[1]) != NEGATIVE) || CAST_TOKEN_VECTOR(args[0])->size() == 0)
		addExceptionMessage(_error);
	else if (!CAST_RULE(args[2])->getAllAntecedents().empty())
		addExceptionMessage("Unexpected token : line " + std::to_string(CAST_INT(args[4])) + " : the expression must contain only one '=>'.");
	else if (_brackets > 0)
		addExceptionMessage("Unexpected token : line " + std::to_string(CAST_INT(args[4])) + " : '(' not closed.");

	CAST_RULE(args[2])->setAntecedents(*CAST_TOKEN_VECTOR(args[0]));
	CAST_TOKEN_VECTOR(args[0])->clear();
}

void			LexerParser::addOperator(t_vector::iterator i, std::vector<void *> args)//[[0] std::vector<Token> &newTokenList,[1] t_lexem nextLexem]
{
	if (_facts == true || _queries == true)
		addExceptionMessage(_factsAndQueriesError);
	else if (CAST_T_LEXEM(args[1]) != O_BRACKET && CAST_T_LEXEM(args[1]) != OPERAND && CAST_T_LEXEM(args[1]) != NEGATIVE)
		addExceptionMessage(_error);
    else if (CAST_RULE(args[2])->getAllAntecedents().size() > 0 && i->second == XOR)
		addExceptionMessage("Unexpected token : line " + std::to_string(CAST_INT(args[4])) + " : " + i->first + " in resolution not allowed.");
    switch (i->second) {
        case AND :
            CAST_TOKEN_VECTOR(args[0])->push_back(Token(i->second, NULL, &andOperator, false));
        break;
        case OR :
            CAST_TOKEN_VECTOR(args[0])->push_back(Token(i->second, NULL, &orOperator, false));
        break;
        case XOR :
            CAST_TOKEN_VECTOR(args[0])->push_back(Token(i->second, NULL, &xorOperator, false));
        break;
        default :
            std::cout << "ERROR" << std::endl;
        break;
    }
}

void			LexerParser::addFacts(t_vector::iterator i, std::vector<void *> args)//[[0] = 0, [1]t_lexem nextLexem]
{
	static_cast<void>(i);
	if (CAST_T_LEXEM(args[1]) != OPERAND && CAST_T_LEXEM(args[1]) != QUERY)
		addExceptionMessage(_error);
	_facts = true;
}

void			LexerParser::addQueries(t_vector::iterator i, std::vector<void *> args)//[[0]t_lexem nextLexem]
{
	static_cast<void>(i);
	if (CAST_T_LEXEM(args[1]) != OPERAND || _facts == false)
		addExceptionMessage(_error);
	_queries = true;
}

void			LexerParser::addOperand(t_vector::iterator i, std::vector<void *> args)//[[0]std::vector<Token> &newTokenList,[1] t_lexem nextLexem,[2] Rule &newRule,[3] ExpertSystem &expertSystem,[4] int nLines,[5] t_lexem *lastImpliyng]
{
	bool negSign = false;
	if (i - 1 >= _lexedFile.begin() && (i - 1)->second == NEGATIVE)
		negSign = true;

	if (_facts == false && _queries == false)
	{
		if (!CAST_EXPERT_SYSTEM(args[3])->findOperand(i->first.c_str()[0]))
		{
			CAST_EXPERT_SYSTEM(args[3])->addOperand(new Operand(i->first.c_str()[0]));
		}

		CAST_TOKEN_VECTOR(args[0])->push_back(Token(i->second, CAST_EXPERT_SYSTEM(args[3])->findOperand(i->first.c_str()[0]), NULL, negSign));
        
		if ((i + 1) != _lexedFile.end() &&
			(CAST_T_LEXEM(args[1]) == NEGATIVE || CAST_T_LEXEM(args[1]) == O_BRACKET || CAST_T_LEXEM(args[1]) == OPERAND || CAST_T_LEXEM(args[1]) == FACTS))
		{
			if (_brackets > 0)
				addExceptionMessage("Unexpected token : line " + std::to_string(CAST_INT(args[4])) + " : '(' not closed.");

			if (CAST_RULE(args[2])->getAllAntecedents().size() == 0)
			{
				if (CAST_T_LEXEM(args[1]) != IMPLIES && CAST_T_LEXEM(args[1]) != DOUBLE_IMPLIES)
					addExceptionMessage(_error);
				CAST_RULE(args[2])->setAntecedents(*CAST_TOKEN_VECTOR(args[0]));
			}
			else
			{
				if (CAST_T_LEXEM(args[5]) == DOUBLE_IMPLIES)
					CAST_RULE(args[2])->setImplying(DOUBLE_IMPLIES);
				else
					CAST_RULE(args[2])->setImplying(IMPLIES);

				CAST_RULE(args[2])->setConsequents(*CAST_TOKEN_VECTOR(args[0]));
				CAST_RULE(args[2])->reorderTokenArrays();
				CAST_EXPERT_SYSTEM(args[3])->addRule(*CAST_RULE(args[2]));

				CAST_RULE(args[2])->clear(); 
				CAST_T_LEXEM(args[5]) = NONE;
			}
			CAST_TOKEN_VECTOR(args[0])->clear();
		}
	}
	else if (_facts == true && _queries == false)
	{
		if (!CAST_EXPERT_SYSTEM(args[3])->findOperand(i->first.c_str()[0]))
			addExceptionMessage("Unexpected token : line " + std::to_string(CAST_INT(args[4])) + " : " + i->first + " does not exist.");
		CAST_EXPERT_SYSTEM(args[3])->findOperand(i->first.c_str()[0])->setValue(TRUE);
		CAST_EXPERT_SYSTEM(args[3])->findOperand(i->first.c_str()[0])->setIsResolved(TRUE);
		CAST_EXPERT_SYSTEM(args[3])->findOperand(i->first.c_str()[0])->setInitialFact(TRUE);
	}
	else if (_facts == true && _queries == true)
	{
		if (!CAST_EXPERT_SYSTEM(args[3])->findOperand(i->first.c_str()[0]))
			addExceptionMessage("Unexpected token : line " + std::to_string(CAST_INT(args[4])) + " : " + i->first + " does not exist.");
		CAST_EXPERT_SYSTEM(args[3])->pushQuery(CAST_EXPERT_SYSTEM(args[3])->findOperand(i->first.c_str()[0]));
	}
	else
		addExceptionMessage(_error);

}

ExpertSystem	LexerParser::Parser()
{
	ExpertSystem		expertSystem = ExpertSystem();
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
        
		_error = "Unexpected token : line " + std::to_string(nLines) + " : " + i->first + " " + printLexemValue(nextLexem);
		_factsAndQueriesError = "Unexpected token : line " + std::to_string(nLines) + " : facts and queries can only contain letters.";
        
		std::vector<void *> args = std::vector<void *>{
            &newTokenList,
            &nextLexem,
            &newRule,
            &expertSystem,
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
	
	if (expertSystem.getAllRules().empty() || !_facts || !_queries)
		addExceptionMessage("Missing elements.");
	else

	try
	{
		exceptionEmpty();	
	}
	catch(std::exception &e)
	{
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

