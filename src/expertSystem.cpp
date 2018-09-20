#include "expertSystem.hpp"

ExpertSystem::ExpertSystem()
{
}

ExpertSystem::ExpertSystem(ExpertSystem const &rhs)
{
	*this = rhs;
}

ExpertSystem::~ExpertSystem()
{
	_operands.clear();
	_rules.clear();
}

ExpertSystem				&ExpertSystem::operator=(ExpertSystem const &rhs)
{
	const std::vector<Operand *> &tmp1 = rhs.getAllOperands();
	const std::vector<Rule> &tmp2 = rhs.getAllRules();

	for (size_t i = 0; i < tmp1.size(); i++)
		_operands.push_back(tmp1[i]);
	for (size_t i = 0; i < tmp2.size(); i++)
		_rules.push_back(tmp2[i]);
	return *this;
}

const std::vector<Operand *> &ExpertSystem::getAllOperands() const 
{
	return (_operands);
}

Operand						*ExpertSystem::findOperand(const char name)
{
	for (size_t i = 0; i < _operands.size(); i++)
	{
		if (_operands[i]->getName() == name)
			return (&(*(_operands[i])));
	}
	return (NULL);
}

Operand						*ExpertSystem::getOperand(int i) const
{
	if (i >= 0 && static_cast<size_t>(i) < _operands.size())
		return (&(*(_operands[i])));
	return (NULL);
}

void				ExpertSystem::addOperand(Operand *operand)
{
	_operands.push_back(operand);
}

std::vector<Rule>	const &ExpertSystem::getAllRules() const
{
	return (_rules);
}

Rule				*ExpertSystem::getRule(int i)
{
	if (i >= 0 && static_cast<size_t>(i) < _rules.size())
		return (&(_rules[i]));
	return (NULL);
}

void				ExpertSystem::addRule(Rule const & rule)
{
	std::vector<Token> tmpToken = rule.getAllAntecedents();
	Operand *tmpOperand = NULL;
	for (size_t i = 0; i < tmpToken.size(); i++)
	{
		if (tmpToken[i].getType() == OPERAND)
		{
			tmpOperand = findOperand(tmpToken[i].getOperand()->getName());
			if (tmpOperand)
			{
				tmpOperand->addAntecedent(rule);
				if (rule.getImplying() == DOUBLE_IMPLIES)
					tmpOperand->addConsequent(rule);
			}
		}
	}
	tmpToken = rule.getAllConsequents();
	for (size_t i = 0; i < tmpToken.size(); i++)
	{
		if (tmpToken[i].getType() == OPERAND)
		{
			tmpOperand = findOperand(tmpToken[i].getOperand()->getName());
			if (tmpOperand)
			{
				tmpOperand->addConsequent(rule);
				if (rule.getImplying() == DOUBLE_IMPLIES)
					tmpOperand->addAntecedent(rule);
			}
		}
	}
	_rules.push_back(rule);
}

void				ExpertSystem::pushQuery(Operand *operand)
{
	_queries.push(operand);
}

Operand				*ExpertSystem::popQuery()
{
	Operand *nextQuery = NULL;
	if (_queries.size() > 0)
	{
		nextQuery = _queries.front();
		_queries.pop();
	}
	return nextQuery;
}

t_status			ExpertSystem::resolveRule(const Rule &rule, std::vector<Operand *> &path)
{
    std::vector<Token>              token_stack;
    const std::vector<Token>        antecedents = rule.getAllAntecedents();

    std::cout << typeid(antecedents[0]).name() << std::endl;
    std::cout << "A size : " << antecedents.size() << std::endl;
	for (size_t i = 0; i < antecedents.size(); i++)
    {
        std::cout << "________________________" << std::endl;
        if (antecedents[i].getType() == OPERAND)
        {
            if (antecedents[i].getOperand()->getIsResolved() || antecedents[i].getOperand()->getInitialFact() )
                token_stack.push_back(antecedents[i]);
            else
                resolveQuery(*(antecedents[i].getOperand()), path);
        }
        else if (antecedents[i].getType() == AND || antecedents[i].getType() == OR || antecedents[i].getType() == XOR)
        {
            Operand                 tmp_operand('T');
 
            Token tmp_token = Token(OPERAND, &tmp_operand, nullptr, false);

            std::cout << "par ici" << std::endl;
//                tmp_operand.setValue(
                antecedents[i].getFunction();//(token_stack.pop_back(), token_stack.pop_back());
                token_stack.push_back(tmp_token);
        }
    }
	return TRUE;//token_stack.front().getOperand()->getValue();
}

t_status            ExpertSystem::resolveQuery(Operand &query, std::vector<Operand *> &path)
{
	for (std::vector<Operand *>::iterator i = path.begin(); i != path.end(); i++)
	{
		if ((*i)->getName() == query.getName())
        {
            (*i)->setIsResolved(true);
			return (*i)->getValue();
        }
	}
	path.push_back(&query);

    Operand *realOperand = findOperand(query.getName());

    if (!realOperand)
        return FALSE;
	t_status result = realOperand ? realOperand->getValue() : NOT_RESOLVED;
	t_status tmp_result = NOT_RESOLVED;

    const std::vector<Rule> consequents = realOperand->getAllConsequents();
	for (size_t i = 0; i < consequents.size(); i++)
	{
		tmp_result = resolveRule(consequents[i], path);
		if (result == NOT_RESOLVED)
			result = tmp_result;
		else if (result != tmp_result)
			return NOT_RESOLVED;
	}

	return result;
}

void                ExpertSystem::resolveAllQueries()
{
	t_status				result = NOT_RESOLVED;
	std::vector<Operand *>	path;

	while (!_queries.empty())
	{
		result = resolveQuery(*popQuery(), path);
		std::cout << (result == NOT_RESOLVED ?	"NOT_RESOLVED" :
					 result == UNDEFINED ?		"UNDEFINED" :
					 result == TRUE ?			"TRUE" :
												"FALSE")
		<< std::endl;
	}
}

void				ExpertSystem::printOperands()
{
	std::cout << "Operands : " << std::endl;
	for (size_t i = 0; i < _operands.size(); i++)
	{
		std::cout << "name : " << _operands[i]->getName() << ", ";
		std::cout << "value : " << _operands[i]->getValue() << ", "; 
		std::cout << "isResolved : " << _operands[i]->getIsResolved() << std::endl;
		_operands[i]->printRules();
	}
	std::cout << "________________________________" << std::endl;
}

void				ExpertSystem::printRules()
{
	for (size_t i = 0; i < _rules.size(); i++)
	{
		std::cout << " Rule " << i << " : " << std::endl;
		std::cout << "Antecedents : " << std::endl;

		printTokenList(_rules[i].getAllAntecedents());
		std::cout << "Consequents : " << std::endl;
		printTokenList(_rules[i].getAllConsequents());
	}
}
