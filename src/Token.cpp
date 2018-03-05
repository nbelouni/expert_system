#include "Token.hpp"


Token::Token(int const type, const Operand *operand, t_operator f, bool const ino):	_type(type),
																				_operand(operand),
																				_function(f),
																				_is_negative_operand(ino)
{
}

Token::~Token()
{
}

int				Token::getType()
{
	return (_type);
}

const Operand	*Token::getOperand()
{
	return (_operand);
}

int				Token::getFunction()
{
	return (_function);
}

bool			Token::getIsNegativeOperand()
{
	return (_is_negative_operand);
}

