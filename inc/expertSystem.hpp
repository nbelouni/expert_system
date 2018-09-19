
#ifndef EXPERT_SYSTEM_HPP
# define EXPERT_SYSTEM_HPP

#include <iostream>
#include <fstream>
#include <utility>
#include <queue>
#include <string>
#include <vector>
#include <regex>
#include <exception>
#include <stdexcept>



typedef enum	e_lexem
{
	COMMENT,
	NEGATIVE,
	AND,
	OR,
	XOR,
	DOUBLE_IMPLIES,
	IMPLIES,
	OPERAND,
	FACTS,
	QUERY,
	O_BRACKET,
	C_BRACKET,
	ENDL,
	NONE
}				t_lexem;

typedef enum	e_status
{
	FALSE,
	TRUE,
	UNDEFINED,
	NOT_RESOLVED
}				t_status;

#include "Operand.hpp"
#include "Token.hpp"
#include "Rule.hpp"
#include "LexerParser.hpp"

#define CAST_T_LEXEM(x)			(*static_cast<t_lexem *>(x))
#define CAST_TOKEN_VECTOR(x)	(static_cast<std::vector<Token> *>(x))
#define CAST_RULE(x)			(static_cast<Rule *>(x))
#define CAST_EXPERT_SYSTEM(x)	(static_cast<ExpertSystem *>(x))
#define CAST_INT(x)				(*static_cast<int *>(x))

t_status		andOperator(Token const &first, Token const &second);
t_status		orOperator(Token const &first, Token const &second);
t_status		xorOperator(Token const &first, Token const &second);


void				printTokenList(std::vector<Token> newTokenList);
std::string			printLexem(t_lexem lex);
std::string			printLexemValue(t_lexem lex);

class Rule;

class ExpertSystem
{
	private:
		std::vector<Operand *>	_operands;
		std::vector<Rule>		_rules;
		std::queue<Operand *>	_queries;

	public:
		ExpertSystem();
		ExpertSystem(ExpertSystem const &);
		~ExpertSystem();

		ExpertSystem				&operator=(ExpertSystem const &rhs);

		const std::vector<Operand *>	&getAllOperands() const;
		Operand							*findOperand(const char);
		Operand							*getOperand(int i) const;
		void							addOperand(Operand *);

		std::vector<Rule>	const		&getAllRules() const ;
		Rule							*getRule(int i);
		void							addRule(const Rule &);

		void							pushQuery(Operand *);
		Operand							*popQuery();

        t_status			            resolveRule(const Rule &rule, std::vector<Operand *> &path);
        t_status                        resolveQuery(Operand &, std::vector<Operand *> &);
        void                            resolveAllQueries();
/*
**	resolve_query()
**	{
		for all rules in query.operand :
			result = resolve rule
			if result != old_result
				error
		query = result
	}
**
**	reslove all _queries 
**	{
**		foreach _query()
**		{
**			resolve_query()
**		}
**	}
**
*/
		void							printOperands();
		void							printRules();
};

#endif
