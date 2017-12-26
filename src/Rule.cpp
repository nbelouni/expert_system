#include "Rule.hpp"

Rule::Rule()
{
}

Rule::Rule(Rule const &rhs)
{
	*this = rhs;
}
Rule::~Rule()
{
	_antecedents.clear();
	_consequents.clear();
}

std::vector<Token>	Rule::getAllAntecedents()
{
	return (_antecedents);
}

Token				*Rule::getAntecedent(int i)
{
	if (i >= 0 && static_cast<size_t>(i) < _antecedents.size())
		return (&(_antecedents[i]));
	return (NULL);
}

void				Rule::addAntecedent(Token const &rule)
{
	_antecedents.push_back(rule);
}

std::vector<Token>	Rule::getAllConsequents()
{
	return (_consequents);
}

Token				*Rule::getConsequent(int i)
{
	if (i >= 0 && static_cast<size_t>(i) < _consequents.size())
		return (&(_consequents[i]));
	return (NULL);
}

void				Rule::addConsequent(Token const &rule)
{
	_consequents.push_back(rule);
}


