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
		bool		_initial_fact;

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

		bool						getInitialFact() const ;
		void						setInitialFact(bool);

		const std::vector<Rule>		getAllAntecedents() const;
		const Rule					*getAntecedent(int) const;
		void						addAntecedent(Rule const &);

		const std::vector<Rule>		getAllConsequents() const;
		const Rule					*getConsequent(int) const;
		void						addConsequent(Rule const &);

		void						printRules();

		class	OperandAlreadyResolvedException: public std::exception
		{
			private:
				std::string		_message;

			public:
				OperandAlreadyResolvedException(std::string);
				OperandAlreadyResolvedException(OperandAlreadyResolvedException const &);
				virtual ~OperandAlreadyResolvedException() throw();
				virtual const char *what() const throw();
		
			private:
			OperandAlreadyResolvedException &operator=(OperandAlreadyResolvedException const &);
		};
};

#endif
