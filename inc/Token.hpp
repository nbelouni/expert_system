#ifndef	TOKEN_HPP
# define TOKEN_HPP

#include "expertSystem.hpp"

class Operand;

class Token
{
	typedef t_status (*)(Operand const &first, Operand const &second) t_operator;
	private:
		const int		_type;
		const Operand	*_operand;
		t_operator		_function;
		const bool		_is_negative_operand;

	public:
		Token(int const, const Operand *, t_operator const, bool const);
		~Token();

		int				getType();
		const Operand	*getOperand();
		int				getFunctionIndex();
		bool			getIsNegativeOperand();

};

#endif
