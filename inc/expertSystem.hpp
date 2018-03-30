
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
	ENDL
}				t_lexem;

typedef enum	e_status
{
	TRUE,
	FALSE,
	UNDEFINED,
	NOT_RESOLVED
}				t_status;

#include "Operand.hpp"
#include "Token.hpp"
#include "Rule.hpp"
#include "LexerParser.hpp"

t_status		andOperator(t_status const first, t_status const second);
t_status		orOperator(t_status const first, t_status const second);
t_status		xorOperator(t_status const first, t_status const second);


void			printTokenList(std::vector<Token> newTokenList);
std::string		printLexem(t_lexem lex);
std::string		printLexemValue(t_lexem lex);

class Rule;

class ExpertSystem
{
	private:
		std::vector<Operand *>	_operands;
		std::vector<Rule>		_rules;
		std::queue<Operand *>	_queries;
		std::vector<int>		_rulesIds;

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

		void							printOperands();
		void							printRules();

		t_status						resolveQuery(char const);
};

#endif
