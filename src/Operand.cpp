#include "Operand.hpp"

Operand::Operand(const char c): _name(c), _value(NOT_RESOLVED), _is_resolved(false)
{
}

Operand::~Operand()
{
//	_antecedents.clear();
	_consequents.clear();
}

Operand	&Operand::operator=(Operand const &rhs)
{
	_name = rhs.getName();
	_value = rhs.getValue();
	_is_resolved = rhs.getIsResolved();
	return (*this);
}

char		Operand::getName() const 
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
	_consequents.push_back(rule);
}

void				Operand::printRules()
{
	std::cout << "Antecedents : " << std::endl;
	for (size_t i = 0; i < _antecedents.size(); i++)
	{
		std::cout << " Rule " << _antecedents[i].getId() << " : " << std::endl;
		printTokenList(_antecedents[i].getAllAntecedents());
		printTokenList(_antecedents[i].getAllConsequents());
	}

	std::cout << "Consequents : " << std::endl;
	for (size_t i = 0; i < _consequents.size(); i++)
	{
		std::cout << " Rule " << _consequents[i].getId() << " : " << std::endl;
		printTokenList(_consequents[i].getAllAntecedents());
		printTokenList(_consequents[i].getAllConsequents());
	}
}
