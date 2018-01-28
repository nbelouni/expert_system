#ifndef RULE_HPP
# define RULE_HPP

/*
 *	class Expression removed
 */

#include "expertSystem.hpp"

class Rule
{
	private:
		std::vector<Token>	_antecedents;
		std::vector<Token>	_consequents;

	public:
		Rule();
		Rule(Rule const &);
		~Rule();

		Rule				&operator=(Rule const &);
		std::vector<Token> const	&getAllAntecedents() const ;
		Token				*getAntecedent(int);
		void				addAntecedent(Token const &);

		std::vector<Token> const &getAllConsequents() const ;
		Token				*getConsequent(int);
		void				addConsequent(Token const &);

		bool	Resolve();
		void	Assign();
};

#endif
