#ifndef OPERAND_HPP
# define OPERAND_HPP

#include "expertSystem.hpp"

typedef int Token;

class Operand
{
	private:
		const char	_name;
		bool		_value;
		bool		_is_verified;
		std::vector<Token>	_antecedents;
		std::vector<Token>	_consequents;

	public:
		Operand(const char);
		~Operand();

		char				getName();

		bool				getValue();
		void				setValue(bool);
                        
		bool				getIsVerified();
		void				setIsVerified(bool);

		std::vector<Token>	getAllAntecedents();
		Token				*getAntecedent(int);
		void				addAntecedent(Token const &);

		std::vector<Token>	getAllConsequents();
		Token				*getConsequent(int);
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
