#include "Expression.hpp"

Expression::Expression(std::vector<Token> const &array) : _tokens(array)
{
}

Expression::~Expression()
{
	_tokens.clear();
}

std::vector<Token>	Expression::getTokens()
{
	return (_tokens);
}

bool	Expression::Resolve()
{
	std::cout << "Revolve." << std::endl;
	return (false);
}

void	Expression::Assign()
{
	std::cout << "Assign values." << std::endl;
}
