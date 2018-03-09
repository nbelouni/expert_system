#ifndef	TOKEN_HPP
# define TOKEN_HPP

#include "expertSystem.hpp"

class Operand;

class Token
{
	typedef bool (*t_operator)(Operand const &first, Operand const &second) ;
	private:
		const t_lexem	_type;
		const Operand	*_operand;
		t_operator		_function;
		const bool		_is_negative_operand;

	public:
		Token(t_lexem const, const Operand *, t_operator, bool const);
		~Token();

		t_lexem			getType() const;
		const Operand	*getOperand();
		t_operator		getFunction();
		bool			getIsNegativeOperand();

};

#endif
