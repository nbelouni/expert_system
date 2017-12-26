#ifndef RULE_HPP
# define RULE_HPP

/*
 *	REVOIR CETTE CLASSE ET EXPRESSION : CHOISIR + CLEAN
 *	(REVOIR AUSSI Operand.hpp)
 *
 */

class Rule
{
	private:
		std::vector<Token>	_antecedent;
		std::vector<Token>	_consequent;

	public:
		Rule();
		Rule(Rule const &);
		~Rule();

		getAllAntecedent();
		getAntecedent(int);
		addAntecedent(Token const &);

		getAllConsequent();
		getConsequent(int);
		addConsequent(Token const &);
}

#endif
