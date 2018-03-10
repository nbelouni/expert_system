#include "Token.hpp"


Token::Token(t_lexem const type, const Operand *operand, t_operator f, bool const ino):	_type(type),
																				_operand(operand),
																				_function(f),
																				_is_negative_operand(ino)
{
}

Token::~Token()
{
}

t_lexem				Token::getType() const
{
	return (_type);
}

const Operand		*Token::getOperand() const
{
	return (&(*_operand));
}

Token::t_operator	Token::getFunction() const
{
	return (_function);
}

bool				Token::getIsNegativeOperand() const
{
	return (_is_negative_operand);
}

