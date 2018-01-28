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

Rule				&Rule::operator=(Rule const &rhs)
{
	const std::vector<Token> &tmp1 = rhs.getAllAntecedents();
	const std::vector<Token> &tmp2 = rhs.getAllConsequents();

	for (size_t i = 0; i < tmp1.size(); i++)
		_antecedents.push_back(tmp1[i]);
	for (size_t i = 0; i < tmp2.size(); i++)
		_consequents.push_back(tmp2[i]);
	return *this;
}

const std::vector<Token> &Rule::getAllAntecedents() const 
{
	return (_antecedents);
}

Token				*Rule::getAntecedent(int i)
{
	Token *ret = NULL;
	if (i >= 0 && static_cast<size_t>(i) < _antecedents.size())
		ret = new Token(_antecedents[i]);
	return (ret);
}

void				Rule::addAntecedent(Token const &rule)
{
	_antecedents.push_back(rule);
}

std::vector<Token>	const &Rule::getAllConsequents() const
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


bool				Rule::Resolve()
{
	std::cout << "Revolve." << std::endl;
	return (false);
}

void				Rule::Assign()
{
	std::cout << "Assign values." << std::endl;
}
