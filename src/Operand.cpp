#include "Operand.hpp"

Operand::Operand(const char c): _name(c), _value(false), _is_resolved(false)
{
}

Operand::~Operand()
{
//	_antecedents.clear();
	_consequents.clear();
}

char	Operand::getName() const 
{
	return (_name);
}

bool		Operand::getValue() const 
{
	return (_value);
}

void		Operand::setValue(bool value)
{
	_value = value;
}

bool		Operand::getIsResolved() const 
{
	return (_is_resolved);
}

void		Operand::setIsResolved(bool value)
{
	_is_resolved = value;
}
/*
std::vector<Token>	Operand::getAllAntecedents()
{
	return (_antecedents);
}

Token				*Operand::getAntecedent(int i)
{
	if (i >= 0 && static_cast<size_t>(i) < _antecedents.size())
		return (&(_antecedents[i]));
	return (NULL);
}

void				Operand::addAntecedent(Token const &rule)
{
	_antecedents.push_back(rule);
}
*/
const std::vector<Token>	Operand::getAllConsequents()
{
	return (_consequents);
}

const Token				*Operand::getConsequent(int i)
{
	if (i >= 0 && static_cast<size_t>(i) < _consequents.size())
		return (&(_consequents[i]));
	return (NULL);
}

void				Operand::addConsequent(Token const &rule)
{
	_consequents.push_back(rule);
}



/*
 *	Nested Classes
 */

Operand::OperandAlreadyResolvedException::OperandAlreadyResolvedException(std::string s) : _message(s)
{
}

Operand::OperandAlreadyResolvedException::OperandAlreadyResolvedException(OperandAlreadyResolvedException const &i)
{
	*this = i;
}

Operand::OperandAlreadyResolvedException::~OperandAlreadyResolvedException() throw()
{
	_message.clear();
}

Operand::OperandAlreadyResolvedException &Operand::OperandAlreadyResolvedException::operator=(OperandAlreadyResolvedException const &d)
{
	(void)d;
	return *this;
}

char const	*Operand::OperandAlreadyResolvedException::what() const throw()
{
	return (_message.c_str());
}
