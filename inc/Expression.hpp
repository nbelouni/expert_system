#ifndef EXPRESSION_HPP
# define EXPRESSION_HPP

#include "expertSystem.hpp"

class Expression
{
	private:
		std::vector<Token>	_tokens;

	public:
		Expression(std::vector<Token> const &);
		~Expression();

		std::vector<Token>	getTokens();

		bool	Resolve();
		void	Assign();
};

#endif
