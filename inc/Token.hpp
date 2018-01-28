#ifndef	TOKEN_HPP
# define TOKEN_HPP

#include "expertSystem.hpp"

class Operand;

class Token
{
	private:
		const int		_type;
		const Operand	*_operand;
		const int		_function_index;
		const bool		_is_negative_operand;

	public:
		Token(int const, const Operand *, int const, bool const);
		~Token();

		int				getType();
		const Operand	*getOperand();
		int				getFunctionIndex();
		bool			getIsNegativeOperand();
};

#endif
