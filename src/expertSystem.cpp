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
				tmpOperand->addAntecedent(rule);
		}
	}
	tmpToken = rule.getAllConsequents();
	for (size_t i = 0; i < tmpToken.size(); i++)
	{
		if (tmpToken[i].getType() == OPERAND)
		{
			tmpOperand = findOperand(tmpToken[i].getOperand()->getName());
			if (tmpOperand)
				tmpOperand->addConsequent(rule);
		}
	}
	_rules.push_back(rule);
}

int					ExpertSystem::getNextRuleId(Operand const * const op) const
{
	size_t	i;
	size_t	j;

	if (op->getAllConsequents().size() > 0 && _rulesIds.size() == 0)
		return 0;
	for (i = 0; i < op->getAllConsequents().size(); i++)
	{
		for (j = 0; j < _rulesIds.size(); j++)
		{
			if (op->getConsequent(i)->getId() != _rulesIds[j])
				return i;
		}
	}
	return -1;
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
		std::cout << " Rule " << _rules[i].getId() << " : " << std::endl;
		std::cout << "Antecedents : " << std::endl;
		printTokenList(_rules[i].getAllAntecedents());
		std::cout << "Consequents : " << std::endl;
		printTokenList(_rules[i].getAllConsequents());
	}
}

t_status			ExpertSystem::resolveQuery(char const operandName)
{
	Operand					*currentOperand;
	int						currentId;
	const Rule				*currentRule;
	std::list<t_status>	operandsStatus;

	currentOperand = findOperand(operandName);

	currentId = getNextRuleId(currentOperand);

//	std::cout << "currentId : " << currentId << std::endl;
	if (currentId == -1)
	{
		if (currentOperand->getValue() == NOT_RESOLVED)
			return FALSE;
		else
			return currentOperand->getValue();
	}

	currentRule = currentOperand->getConsequent(currentId);
//	if (!currentRule)
//		std::cout << "NO RULES " << std::endl;
	_rulesIds.push_back(currentRule->getId());

//		std::cout << "____0" << std::endl;
	std::vector<Token>::const_iterator i;
	for (i = currentRule->getAllAntecedents().begin(); i != currentRule->getAllAntecedents().end(); i++)
	{
		if (i->getType() == OPERAND)
		{
//		std::cout << "____1" << std::endl;
			if (i->getOperand()->getValue() == NOT_RESOLVED)
			{
				operandsStatus.push_back(resolveQuery(i->getOperand()->getName()));
			}
			else
			{
//		std::cout << "____2" << std::endl;
				t_status status = i->getOperand()->getValue();
	
				if (i->getIsNegativeOperand() == true)
				{
					if (i->getOperand()->getValue() == TRUE)
						status = FALSE;
					else
						status = TRUE;
					
				}
				operandsStatus.push_back(status);
			}
		}
		else
		{
//		std::cout << "____3.0" << std::endl;
//		std::cout << operandsStatus.size() << std::endl;
//		std::cout << *operandsStatus.begin() << ", " << *(std::next(operandsStatus.begin())) << std::endl;
			t_status tmp = i->getFunction() (*operandsStatus.begin(), *(std::next(operandsStatus.begin())));
//		std::cout << "____3.1" << std::endl;
			operandsStatus.pop_front();
//		std::cout << "____3.2" << std::endl;
			operandsStatus.pop_front();
//		std::cout << "____3.3" << std::endl;
			operandsStatus.push_front(tmp);
//		std::cout << "____3.4" << std::endl;
		}
	}
	std::cout << operandsStatus.size() << std::endl;
	if (currentOperand->getIsResolved() == 1 && operandsStatus.front() != currentOperand->getValue())
	{
		throw new OperandAlreadyResolvedException("Contradictory facts.");
	}
	currentOperand->setValue(operandsStatus.front());
	currentOperand->setIsResolved(true);
	return operandsStatus.front();
}


/*
 *	Nested Classes
 */

ExpertSystem::OperandAlreadyResolvedException::OperandAlreadyResolvedException(std::string s) : _message(s)
{
}

ExpertSystem::OperandAlreadyResolvedException::OperandAlreadyResolvedException(OperandAlreadyResolvedException const &i)
{
	*this = i;
}

ExpertSystem::OperandAlreadyResolvedException::~OperandAlreadyResolvedException() throw()
{
	_message.clear();
}

ExpertSystem::OperandAlreadyResolvedException &ExpertSystem::OperandAlreadyResolvedException::operator=(OperandAlreadyResolvedException const &d)
{
	(void)d;
	return *this;
}

char const	*ExpertSystem::OperandAlreadyResolvedException::what() const throw()
{
	return (_message.c_str());
}
