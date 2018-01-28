
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
#include "Rule.hpp"
#include "LexerParser.hpp"

class Rule;

class ExpertSystem
{
	private:
		std::vector<Operand>	_operands;
		std::vector<Rule>		_rules;

	public:
		ExpertSystem();
		ExpertSystem(ExpertSystem const &);
		~ExpertSystem();

		ExpertSystem				&operator=(ExpertSystem const &rhs);

		const std::vector<Operand>	&getAllOperands() const;
		Operand						*findOperand(const char);
		Operand						*getOperand(int i);
		void						addOperand(const Operand &);

		std::vector<Rule>	const	&getAllRules() const ;
		Rule						*getRule(int i);
		void						addRule(const Rule &);
};

#endif
