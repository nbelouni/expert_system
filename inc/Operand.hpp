#ifndef OPERAND_HPP
# define OPERAND_HPP

#include "expertSystem.hpp"

class Rule;
class Token;

class Operand
{
	private:
		char		_name;
		t_status	_value;
		bool		_is_resolved;

		std::vector<Rule>	_antecedents;
		std::vector<Rule>	_consequents;

	public:
		Operand(const char);
		~Operand();

		Operand						&operator=(Operand const &);

		char						getName() const ;

		t_status					getValue() const ;
		void						setValue(t_status);

		bool						getIsResolved() const;
		void						setIsResolved(bool);

		const std::vector<Rule>		getAllAntecedents() const;
		const Rule					*getAntecedent(int) const;
		void						addAntecedent(Rule const &);

		const std::vector<Rule>		getAllConsequents() const;
		const Rule					*getConsequent(int) const;
		void						addConsequent(Rule const &);

		void						printRules();

};

#endif
