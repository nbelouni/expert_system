#include "Rule.hpp"

int Rule::_nRules = 0;

Rule::Rule()
{
	_id = _nRules;
	_nRules++;
}

Rule::Rule(Rule const &rhs)
{
	*this = rhs;
//	_id = _nRules;
//	_nRules++;
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

	_id = rhs.getId();
	for (size_t i = 0; i < tmp1.size(); i++)
		_antecedents.push_back(tmp1[i]);
	for (size_t i = 0; i < tmp2.size(); i++)
		_consequents.push_back(tmp2[i]);
	return *this;
}


int 				Rule::getId() const
{
	return _id;
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


void				Rule::setAntecedents(std::vector<Token> rhs)
{
	for (size_t i = 0; i < rhs.size(); i++)
		this->addAntecedent(rhs[i]);
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

void				Rule::setConsequents(std::vector<Token> rhs)
{
	for (size_t i = 0; i < rhs.size(); i++)
		this->addConsequent(rhs[i]);
}

void				Rule::reorderTokenArrays()
{
	std::vector<Token>	finalOrder;
	std::vector<Token>	operators;

	std::vector<Token>::iterator i;

	for (i = _antecedents.begin(); i != _antecedents.end(); i++)
	{
		if (i->getType() == OPERAND)
			finalOrder.push_back(*i);
		else if (i->getType() != C_BRACKET && i->getType() != O_BRACKET)
		{
			if (operators.empty() || operators.back().getType() > i->getType())
				operators.push_back(*i);
			else
			{
				while (!operators.empty() && operators.back().getType() < i->getType())
				{
					finalOrder.push_back(operators.back());
					operators.pop_back();
				}
				operators.push_back(*i);
			}
		}
		else if (i->getType() == O_BRACKET)
			operators.push_back(*i);
		else if (i->getType() == C_BRACKET)
		{
			while (!operators.empty() && operators.back().getType() != O_BRACKET)
			{
				finalOrder.push_back(operators.back());
				operators.pop_back();
			}
			if (!operators.empty())
				operators.pop_back();
		}
	}
	while (!operators.empty())
	{
		finalOrder.push_back(operators.back());
		operators.pop_back();
	}
	_antecedents = finalOrder;
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

void				Rule::clear()
{
	_antecedents.clear();
	_consequents.clear();
}
