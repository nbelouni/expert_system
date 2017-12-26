
#ifndef EXPERT_SYSTEM_HPP
# define EXPERT_SYSTEM_HPP

#include <iostream>
#include <fstream>
#include <utility>
#include <string>
#include <vector>
#include <regex>



typedef enum	e_lexem
{
	COMMENT,
	O_BRACKET,
	C_BRACKET,
	NEGATIVE,
	AND,
	OR,
	XOR,
	DOUBLE_IMPLIES,
	IMPLIES,
	OPERAND,
	FACTS,
	QUERY,
	ENDL
}				t_lexem;

#include "Operand.hpp"
#include "Token.hpp"
#include "Expression.hpp"
#include "Rule.hpp"
#include "LexerParser.hpp"

#endif
