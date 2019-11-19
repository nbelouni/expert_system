#include "Operand.hpp"

Operand::Operand(const char c): _name(c), _value(FALSE), _is_resolved(false), _initial_fact(false)
{
}

Operand::~Operand()
{
	_antecedents.clear();
	_consequents.clear();
}

Operand	&Operand::operator=(Operand const &rhs)
{
	_name = rhs.getName();
	_value = rhs.getValue();
	_is_resolved = rhs.getIsResolved();
	return (*this);
}

char	Operand::getName() const 
{
	return (_name);
}

t_status	Operand::getValue() const 
{
	return (_value);
}

void		Operand::setValue(t_status value)
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

bool		Operand::getInitialFact() const
{
    return _initial_fact;
}

void	    Operand::setInitialFact(bool value)
{
    _initial_fact = value;
}

const std::vector<Rule>	Operand::getAllAntecedents() const
{
	return (_antecedents);
}

const Rule				*Operand::getAntecedent(int i) const
{
	if (i >= 0 && static_cast<size_t>(i) < _antecedents.size())
		return (&(_antecedents[i]));
	return (NULL);
}

void				Operand::addAntecedent(Rule const &rule)
{
	_antecedents.push_back(rule);
}

const std::vector<Rule>	Operand::getAllConsequents() const
{
	return (_consequents);
}

const Rule				*Operand::getConsequent(int i) const
{
	if (i >= 0 && static_cast<size_t>(i) < _consequents.size())
		return (&(_consequents[i]));
	return (NULL);
}

void				Operand::addConsequent(Rule const &rule)
{
	if (rule.getContainsXor())
	{
		std::vector<Rule> tmp;
		tmp.push_back(rule);
		tmp.insert(tmp.end(), _consequents.begin(), _consequents.end());
		_consequents.clear();
		_consequents = tmp;
	}
	else
		_consequents.push_back(rule);
}

void				Operand::printRules()
{
	std::cout << "Antecedents : " << std::endl;
	for (size_t i = 0; i < _antecedents.size(); i++)
	{
		std::cout << " Rule " << i << " : " << std::endl;
		printTokenList(_antecedents[i].getAllAntecedents(), false);
		printTokenList(_antecedents[i].getAllConsequents(), true);
	}

	std::cout << "Consequents : " << std::endl;
	for (size_t i = 0; i < _consequents.size(); i++)
	{
		std::cout << " Rule " << i << " : " << std::endl;
		printTokenList(_consequents[i].getAllAntecedents(), false);
		printTokenList(_consequents[i].getAllConsequents(), true);
	}
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
