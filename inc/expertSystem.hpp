
#ifndef EXPERT_SYSTEM_HPP
# define EXPERT_SYSTEM_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <utility>
#include <queue>
#include <string>
#include <vector>
#include <regex>
#include <exception>
#include <stdexcept>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>



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
	NOT_RESOLVED,
    ERROR
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

t_status    		andOperator(Token const &, Token const &);
t_status    		orOperator(Token const &, Token const &);
t_status	    	xorOperator(Token const &, Token const &);

t_status            assignAnd(Token const &, Token const &, t_status);
t_status            assignOr(Token const &, Token const &, t_status);
void                assignValue(Token const &, t_status);

void				printTokenList(std::vector<Token> newTokenList);
std::string			printLexem(t_lexem lex);
std::string			printLexemValue(t_lexem lex);
void                printStatus(t_status result);
const char *        statusToString(t_status result);

class Rule;

class ExpertSystem
{
	private:
		std::vector<Operand *>	    _operands;
		std::vector<Rule>		    _rules;
		std::queue<Operand *>	    _queries;

	public:
		ExpertSystem();
		ExpertSystem(ExpertSystem const &);
		~ExpertSystem();

		ExpertSystem				&operator=(ExpertSystem const &rhs);

		void							clear();

		const std::vector<Operand *>	&getAllOperands() const;
		Operand							*findOperand(const char);
		Operand							*getOperand(int i) const;
		void							addOperand(Operand *);

		std::vector<Rule>const			&getAllRules() const ;
		Rule							*getRule(int i);
		void							addRule(const Rule &);

		void							pushQuery(Operand *);
		Operand							*popQuery();

        void				            assignValues(const std::vector<Token> facts, Token result);
        Token                           getFactStatus(const std::vector<Token> facts, std::vector<Operand *> &path);
        t_status			            resolveRule(const Rule &rule, std::vector<Operand *> &path, Operand &);
        t_status                        resolveQuery(Operand &, std::vector<Operand *> &);
        void                            resolveAllQueries();
		void							printOperands();
		void							printRules();
		void							printRule(Rule );
		class	CannotBeResolvedException : public std::exception
		{
			private:
				std::string         _message;

			public:
				CannotBeResolvedException(std::string error);
				CannotBeResolvedException(CannotBeResolvedException const &);
				virtual ~CannotBeResolvedException() throw();
				virtual const char *what() const throw();
		
			private:
			CannotBeResolvedException &operator=(CannotBeResolvedException const &);
		};
};

#endif
