#include "Rule.hpp"

Rule::Rule()
{
	_contains_xor = false;
}

Rule::Rule(Rule const &rhs)
{
	*this = rhs;
}
Rule::~Rule()
{
	_antecedents.clear();
	_consequents.clear();
	_to_string.clear();
}

Rule				&Rule::operator=(Rule const &rhs)
{
	const std::vector<Token> &tmp1 = rhs.getAllAntecedents();
	const std::vector<Token> &tmp2 = rhs.getAllConsequents();

	_contains_xor = rhs.getContainsXor();
	_implying = rhs.getImplying();
	_to_string = rhs.getToString();
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


void				Rule::setAntecedents(std::vector<Token> rhs)
{
	for (size_t i = 0; i < rhs.size(); i++)
	{
		this->addAntecedent(rhs[i]);
	}
}


void				Rule::setImplying(t_lexem l)
{
	_implying = l;
}

t_lexem				Rule::getImplying() const
{
	return _implying;
}

void				Rule::setToString(std::string s)
{
	_to_string = s;
}

std::string			Rule::getToString() const
{
	return (_to_string);
}

bool				Rule::getContainsXor() const
{
	return _contains_xor;
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
	if (rule.getType() == XOR)
		_contains_xor = true;
	_consequents.push_back(rule);
}

void				Rule::setConsequents(std::vector<Token> rhs)
{
	for (size_t i = 0; i < rhs.size(); i++)
		this->addConsequent(rhs[i]);
}

void                Rule::reorderArray(std::vector<Token> &array)
{
	std::vector<Token>	finalOrder;
	std::vector<Token>	operators;

	std::vector<Token>::iterator i;
	
	for (i = array.begin(); i != array.end(); i++)
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
	array = finalOrder;
}

void				Rule::reorderTokenArrays()
{
    reorderArray(_antecedents);
    reorderArray(_consequents);
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
	_contains_xor = false;
}
