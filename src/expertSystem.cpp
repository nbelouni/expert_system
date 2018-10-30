#include "expertSystem.hpp"

/*
**      Construct / destroy
*/
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

/*
**      GET / SET / FIND
*/
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

/*
**     Resolve Rule
**     return found Operand value
*/

t_status            ExpertSystem::findStatus(const std::vector<Token> fact, std::vector<Operand *> &path)
{
    std::vector<Token>  token_stack;

	for (size_t i = 0; i < fact.size(); i++)
    {
        Operand             *tmp_operand = new Operand('#');
        Token tmp_token = Token(OPERAND, tmp_operand, nullptr, false);
        if (fact[i].getType() == OPERAND)
        {
            if (!fact[i].getOperand()->getIsResolved() && !fact[i].getOperand()->getInitialFact() )
                resolveQuery(*(fact[i].getOperand()), path);
            token_stack.push_back(fact[i]);
        }
        else if (fact[i].getType() == AND || fact[i].getType() == OR || fact[i].getType() == XOR)
        {
            Token tmp = token_stack.back();
            std::vector<Token>::iterator tmp1 = token_stack.erase(token_stack.end() - 3,token_stack.end() - 1);
            tmp_token.getOperand()->setValue(fact[i].getFunction()(tmp1[0], tmp1[1]));
            token_stack.push_back(tmp_token);
        }
    }
    
    return token_stack.front().getOperand()->getValue();
}

void                ExpertSystem::setStatus(const std::vector<Token> fact, t_status result)
{
    std::vector<Token>  token_stack;

    for (size_t i = 0; i < fact.size(); i++)
    {
        Token tmp_token = Token(OPERAND, new Operand('T'), nullptr, false);
		if (fact[i].getType() == OPERAND)
        {
           	token_stack.push_back(fact[i]);
        }
		else if (fact[i].getType() == AND)
		{
	            std::vector<Token>::iterator tmp1 = token_stack.erase(token_stack.end() - 3,token_stack.end() - 1);
	            tmp_token.getOperand()->setValue(assignAnd(tmp1[0], tmp1[1], result));
                tmp_token.getOperand()->setIsResolved(true);
            	token_stack.push_back(tmp_token);
		}
		else if (fact[i].getType() == OR)
		{
	            std::vector<Token>::iterator tmp1 = token_stack.erase(token_stack.end() - 3,token_stack.end() - 1);
	            tmp_token.getOperand()->setValue(assignAnd(tmp1[0], tmp1[1], result));
                tmp_token.getOperand()->setIsResolved(true);
            	token_stack.push_back(tmp_token);
		}
    }
    if (token_stack.size() != 1)
        std::cout << "ERROR" << std::endl;
    assignValue(token_stack.front(), result);
}

t_status			ExpertSystem::resolveRule(const Rule &rule, std::vector<Operand *> &path, char operand_name)
{
	t_status					result;

	result = findStatus(rule.getAllAntecedents(), path);

	if (result == FALSE)
		return NOT_RESOLVED;

    setStatus(rule.getAllConsequents(), result);

	return findOperand(operand_name)->getValue();
}

/*
**  Resolve all Rules that match with Operand as result
*/
t_status            ExpertSystem::resolveQuery(Operand &query, std::vector<Operand *> &path)
{
	for (std::vector<Operand *>::iterator i = path.begin(); i != path.end(); i++)
	{
		if ((*i)->getName() == query.getName())
        {
            (*i)->setIsResolved(true);
            std::cout << "in path." << std::endl;
			return (*i)->getValue();
        }
	}
    std::cout << "not in path." << std::endl;
	path.push_back(&query);

    Operand *realOperand = findOperand(query.getName());

    if (!realOperand)
    {
        std::cout << "no operand found" << std::endl;
        return FALSE;
    }
    else
        std::cout << "operand found" << std::endl;
    const std::vector<Rule> consequents = realOperand->getAllConsequents();
    if (consequents.size() == 0)
    {
        realOperand->setIsResolved(true);
        if (realOperand->getValue() == NOT_RESOLVED)
            realOperand->setValue(FALSE);
    }

	t_status result = realOperand->getIsResolved() ? realOperand->getValue() : NOT_RESOLVED;
	t_status tmp_result = NOT_RESOLVED;

	for (size_t i = 0; i < consequents.size(); i++)
	{
		tmp_result = resolveRule(consequents[i], path, realOperand->getName());
		if (result == NOT_RESOLVED)
			result = tmp_result;
		else if (result != tmp_result)
			return ERROR;
	}

    std::cout << "here" << std::endl;
	return realOperand->getValue();
}

/*
**  Call resolveQuery() on each query.
*/
void                ExpertSystem::resolveAllQueries()
{
	t_status				    result = NOT_RESOLVED;
	std::vector<Operand *>	    path;
    std::stringstream           ss;
    std::vector<std::string>    results;

	while (!_queries.empty())
	{
        char c = _queries.front()->getName();
		result = resolveQuery(*popQuery(), path);
        printStatus(result);
        std::cout << result << " == " << ERROR << std::endl;
        if (result == ERROR)
        {
            path.clear();
            results.clear();
            ss << "Error : the fact \"" << c << "\" cannot be resolved";
            throw CannotBeResolvedException(ss.str());
        }
        else
            ss << "Query name : " << c << ", result : " << statusToString(findOperand(c)->getValue());
        results.push_back(ss.str());
        ss.str("");
        path.clear();
	}

    for (size_t i = 0; i < results.size(); i++)
        std::cout << results[i] << std::endl;
    results.clear();
}


/*
**  Debug / Log
*/
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

void				ExpertSystem::printRule(Rule r)
{
		std::cout << "Antecedents : " << std::endl;

		printTokenList(r.getAllAntecedents());
		std::cout << "Consequents : " << std::endl;
		printTokenList(r.getAllConsequents());
}

/*
**  Exception
*/
ExpertSystem::CannotBeResolvedException::CannotBeResolvedException(std::string error) : _message(error)
{
}

ExpertSystem::CannotBeResolvedException::CannotBeResolvedException(CannotBeResolvedException const &i)
{
	*this = i;
}

ExpertSystem::CannotBeResolvedException::~CannotBeResolvedException() throw()
{
}

ExpertSystem::CannotBeResolvedException &ExpertSystem::CannotBeResolvedException::operator=(CannotBeResolvedException const &d)
{
	(void)d;
	return *this;
}

char const	*ExpertSystem::CannotBeResolvedException::what() const throw()
{
	return (_message.c_str());
}

