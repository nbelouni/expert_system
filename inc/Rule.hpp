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
		t_lexem				_implying;
		bool				_contains_xor;
		std::string			_to_string;
	public:
		Rule();
		Rule(Rule const &);
		~Rule();

		Rule				&operator=(Rule const &);
		std::vector<Token> const	&getAllAntecedents() const ;
		Token				*getAntecedent(int);
		void				addAntecedent(Token const &);
		void				setAntecedents(std::vector<Token>);

		void				setImplying(t_lexem l);
		t_lexem				getImplying() const;

		void				setToString(std::string s);
		std::string			getToString() const;

		bool				getContainsXor() const;

		std::vector<Token> const &getAllConsequents() const ;
		Token				*getConsequent(int);
		void				addConsequent(Token const &);
		void				setConsequents(std::vector<Token>);
        void                reorderArray(std::vector<Token> &array);
		void				reorderTokenArrays();

		bool	Resolve();
		void	Assign();

		void	clear();
};

#endif
