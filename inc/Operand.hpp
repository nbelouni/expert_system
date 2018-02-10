#ifndef OPERAND_HPP
# define OPERAND_HPP

#include "expertSystem.hpp"

class Token;

class Operand
{
	private:
		const char	_name;
		bool		_value;
		bool		_is_resolved;
/*
**		Pas utile
**		
**		std::vector<Token>	_antecedents;
**		
*/
		std::vector<Token>	_consequents;

	public:
		Operand(const char);
		~Operand();

		char			getName() const ;

		bool			getValue() const ;
		void				setValue(bool);
                        
		bool			getIsResolved() const;
		void				setIsResolved(bool);
/*
		const std::vector<Token>	getAllAntecedents();
		const Token			*getAntecedent(int);
		void				addAntecedent(Token const &);
*/
		const std::vector<Token>	getAllConsequents();
		const Token				*getConsequent(int);
		void				addConsequent(Token const &);

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
