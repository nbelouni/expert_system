#include "Token.hpp"


Token::Token(int const type, const Operand *operand, int const fi, bool const ino):	_type(type),
																				_operand(operand),
																				_function_index(fi),
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

int				Token::getFunctionIndex()
{
	return (_function_index);
}

bool			Token::getIsNegativeOperand()
{
	return (_is_negative_operand);
}
