#include "expertSystem.hpp"

/*
**      Construct / destroy
*/
static int tabs = 0;
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
**      GET / SET / ADD / FIND
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

std::vector<Token>	ExpertSystem::createAntsFromXor(Token token, bool in, std::vector<Token> array)
{
	std::vector<Token>	newAntecedents;

	newAntecedents.push_back(Token(O_BRACKET, NULL, NULL, false));
	newAntecedents.insert(newAntecedents.end(), array.begin(), array.end());
	newAntecedents.push_back(Token(C_BRACKET, NULL, NULL, false));
	newAntecedents.push_back(Token(AND, NULL, &andOperator, false));
	
	if (in == true)
	{
		if (token.getIsNegativeOperand() == true)
			token.setIsNegativeOperand(false);
		else
			token.setIsNegativeOperand(true);
	}
	newAntecedents.push_back(token);
	return (newAntecedents);
}

std::vector<Token>	ExpertSystem::createConsFromXor(Token token, bool in)
{
	std::vector<Token>	newConsequents;

	if (in == false)
	{
		if (token.getIsNegativeOperand() == false)
			token.setIsNegativeOperand(true);
		else
			token.setIsNegativeOperand(false);
	}
	newConsequents.push_back(token);
	return (newConsequents);
}
void				ExpertSystem::createRuleFromXor(const Rule &rule)
{
	Rule				tmpRule;
	std::vector<Token>	ants = rule.getAllAntecedents();//hahaha
	Token				first = rule.getAllConsequents()[0];
	Token				second = rule.getAllConsequents()[2];
	
	/*
	**	Xxx => B ^ C
	**	1st rule  : (Xxx) + B => !C
	**	2nd rule  : (Xxx) + !B => C
	**	3rd rule  : (Xxx) + C => !B
	**	4th rule  : (Xxx) + !C => B
	*/
	tmpRule.setAntecedents(createAntsFromXor(first, false, rule.getAllAntecedents()));
	tmpRule.setConsequents(createConsFromXor(second, false));
	tmpRule.setImplying(rule.getImplying());
	addRule(tmpRule);

	std::cout << "-" << tmpRule.getToString();

	tmpRule.clear();

	tmpRule.setAntecedents(createAntsFromXor(first, true, rule.getAllAntecedents()));
	tmpRule.setConsequents(createConsFromXor(second, true));
	tmpRule.setImplying(rule.getImplying());
	addRule(tmpRule);

	std::cout << "              -" << tmpRule.getToString();

	tmpRule.clear();

	tmpRule.setAntecedents(createAntsFromXor(second, false, rule.getAllAntecedents()));
	tmpRule.setConsequents(createConsFromXor(first, false));
	tmpRule.setImplying(rule.getImplying());
	addRule(tmpRule);

	std::cout << "              -" << tmpRule.getToString();

	tmpRule.clear();

	tmpRule.setAntecedents(createAntsFromXor(second, true, rule.getAllAntecedents()));
	tmpRule.setConsequents(createConsFromXor(first, true));
	tmpRule.setImplying(rule.getImplying());
	addRule(tmpRule);

	std::cout << "              -" << tmpRule.getToString();

	tmpRule.clear();

}

void				ExpertSystem::addRule(Rule &rule)
{
	std::vector<Token>	tmpToken = rule.getAllAntecedents();
	Operand 			*tmpOperand = NULL;

	rule.setToString(ruleToString(rule));
	if (rule.getContainsXor())
	{
		/*
		**	create 4 rules from rule
		*/
		std::cout << rule.getToString() << "IS EQUAL TO : ";
		createRuleFromXor(rule);
	}
	else
	{
		rule.reorderTokenArrays();
		for (size_t i = 0; i < tmpToken.size(); i++)
		{
			if (tmpToken[i].getType() == OPERAND)
			{
				tmpOperand = findOperand(tmpToken[i].getOperand()->getName());
				if (tmpOperand)
				{
					tmpOperand->addAntecedent(rule);
					if (rule.getImplying() == DOUBLE_IMPLIES)
					{
						tmpOperand->addConsequent(rule);
					}
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
		if (rule.getImplying() == DOUBLE_IMPLIES)
		{
			/*
			**	Xxx <=> Yyy
			**	1st rule : Xxx => Yyy
			**	2nd rule : Yyy => Xxx
			*/

			Rule	reverseRule;

			reverseRule.setAntecedents(rule.getAllConsequents());
			reverseRule.setConsequents(rule.getAllAntecedents());
			_rules.push_back(reverseRule);
		}
	}
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
**	Resolve rule
*/

Token				ExpertSystem::getFactStatus(const std::vector<Token> facts, std::vector<Operand *> &path)
{
	/*
	**	get facts
	**	 if not not resolved : 	resolution rule
	**							set fact status
	**							add to facts array
	*/

	printTokenList(facts, true);
	std::vector<Token>				token_stack;
	for (size_t i = 0; i < facts.size(); i++)
	{
		Token tmp_token = Token(OPERAND, new Operand('#'), nullptr, false);
		if (facts[i].getType() == OPERAND)
		{
//			std::cout << "____________1" << std::endl;
			tabs++;
			for (int i_tabs = 0; i_tabs < tabs; i_tabs++)
				std::cout << "    ";
			std::cout << ". Operand " << facts[i].getOperand()->getName() << ":" << std::endl;

			if (!facts[i].getOperand()->getIsResolved() && !facts[i].getOperand()->getInitialFact() )
			{
				resolveQuery(*(facts[i].getOperand()), path);
			}

			if ((facts[i].getOperand()->getValue() == TRUE && facts[i].getIsNegativeOperand()) || (facts[i].getOperand()->getValue() == FALSE && !facts[i].getIsNegativeOperand()))
			{
				tmp_token.getOperand()->setValue(FALSE);
			}
			else if (facts[i].getOperand()->getValue() == UNDEFINED)
			{
				tmp_token.getOperand()->setValue(UNDEFINED);
			}
			else
			{
				tmp_token.getOperand()->setValue(TRUE);
			}
			tmp_token.getOperand()->setIsResolved(TRUE);
			for (int i_tabs = 0; i_tabs < tabs; i_tabs++)
				std::cout << "	";
			std::cout << "Value = " << statusToString(facts[i].getOperand()->getValue()) << std::endl;
			
//			std::cout << "Name = " << tmp_token.getOperand()->getName() << std::endl;
//			std::cout << "Is Negative = " << (tmp_token.getIsNegativeOperand() ? "TRUE" : "FALSE") << std::endl;
//			std::cout << "Value = " << statusToString(tmp_token.getOperand()->getValue()) << std::endl;
//			std::cout << "Is resolved = " << (tmp_token.getOperand()->getIsResolved() ? "TRUE" : "FALSE") << std::endl;
//			token_stack.push_back(facts[i]);
			token_stack.push_back(tmp_token);
			tabs--;
		}
		else if (facts[i].getType() == AND || facts[i].getType() == OR || facts[i].getType() == XOR)
		{
//			std::cout << "____________2" << std::endl;

			Token tmp = token_stack.back();
//			std::cout << "Name = " << tmp.getOperand()->getName() << std::endl;
//			std::cout << "Is Negative = " << (tmp.getIsNegativeOperand() ? "TRUE" : "FALSE") << std::endl;
//			std::cout << "Value = " << statusToString(tmp.getOperand()->getValue()) << std::endl;
//			std::cout << "Is resolved = " << (tmp.getOperand()->getIsResolved() ? "TRUE" : "FALSE") << std::endl;
			token_stack.pop_back();
//			std::cout << "Name = " << token_stack.back().getOperand()->getName() << std::endl;
//			std::cout << "Is Negative = " << (token_stack.back().getIsNegativeOperand() ? "TRUE" : "FALSE") << std::endl;
//			std::cout << "Value = " << statusToString(token_stack.back().getOperand()->getValue()) << std::endl;
//			std::cout << "Is resolved = " << (token_stack.back().getOperand()->getIsResolved() ? "TRUE" : "FALSE") << std::endl;
			tmp_token.getOperand()->setValue(facts[i].getFunction()(tmp, token_stack.back()));
			token_stack.pop_back();
			token_stack.push_back(tmp_token);
		}
	}
	return token_stack.front();
}

void				ExpertSystem::assignValues(const std::vector<Token> consequents, Token result)
{
	std::vector<Token>			token_stack;

	for (size_t i = 0; i < consequents.size(); i++)
	{
		Token tmp_token = Token(OPERAND, new Operand('#'), nullptr, false);
		if (consequents[i].getType() == OPERAND)
		{
		   	token_stack.push_back(consequents[i]);
		}
		else if (consequents[i].getType() == AND)
		{
			std::vector<Token>::iterator tmp1 = token_stack.erase(token_stack.end() - 3,token_stack.end() - 1);
			tmp_token.getOperand()->setValue(assignAnd(tmp1[0], tmp1[1], result.getOperand()->getValue()));
			tmp_token.getOperand()->setIsResolved(true);
			token_stack.push_back(tmp_token);
		}
		else if (consequents[i].getType() == OR)
		{
			std::vector<Token>::iterator tmp1 = token_stack.erase(token_stack.end() - 3,token_stack.end() - 1);
			tmp_token.getOperand()->setValue(assignOr(tmp1[0], tmp1[1], result.getOperand()->getValue()));
			tmp_token.getOperand()->setIsResolved(true);
			token_stack.push_back(tmp_token);
		}
	}

	if (token_stack.size() != 1)
	{
//		std::cout << "________1" << std::endl;
		std::cout << "ERROR" << std::endl;
	}
	assignValue(token_stack.front(), result.getOperand()->getValue());
	
}

t_status			ExpertSystem::resolveRule(const Rule &rule, std::vector<Operand *> &path, Operand &operand)
{
	for (int i_tabs = 0; i_tabs < tabs; i_tabs++)
		std::cout << "    ";
	std::cout << "rule found : " << rule.getToString();

	const std::vector<Token>	consequents = rule.getAllConsequents();
	Token						result = getFactStatus(rule.getAllAntecedents(), path);

	for (int i_tabs = 0; i_tabs < tabs; i_tabs++)
		std::cout << "    ";
	std::cout << " so the equation is " << statusToString(result.getOperand()->getValue()) << std::endl;

//	if (result.getOperand()->getValue() == FALSE || (result.getOperand()->getValue() == TRUE && result.getIsNegativeOperand() == true))
//	{
//		std::cout << std::endl;
////	return operand.getValue();
//		return result.getOperand()->getValue();
//	}
	if (result.getOperand()->getValue())
	{
		assignValues(rule.getAllConsequents(), result);
		return TRUE;
	}
	return NOT_RESOLVED;
	std::cout << " so " << operand.getName() << " is " << statusToString(operand.getValue()) << std::endl;
//	return operand.getValue();
//	return result.getOperand()->getValue();
}

/*
**	Resolve all rules related to querying operand
*/

t_status			ExpertSystem::resolveQuery(Operand &query, std::vector<Operand *> &path)
{
	Operand					*realOperand;
	
	/*
	**	If querying operand is already set, return
	*/
	for (std::vector<Operand *>::iterator i = path.begin(); i != path.end(); i++)
	{
		if ((*i)->getName() == query.getName())
		{
			(*i)->setIsResolved(true);
			std::cout << "Already trying to resolve : " << query.getName() << " set to default value : FALSE" << std::endl;
			return TRUE;
//			return (*i)->getValue();
		}
	}

	/*
	**	If querying operand has not implying relation (cf. subject)
	**	SHOULD NEVER HAPPEN
	*/
	if (!(realOperand = findOperand(query.getName())))
	{
		std::cout << query.getName() << "cannot be solve for now" << std::endl;
		return FALSE;
	}

	path.push_back(&query);
	const std::vector<Rule> consequents = realOperand->getAllConsequents();

	/*
	**	If querying operand has not implying relation (cf. subject)
	*/
	if (consequents.size() == 0)
	{
		realOperand->setIsResolved(true);
		for (int i_tabs = 0; i_tabs < tabs; i_tabs++)
			std::cout << "    ";
		std::cout << "No rule assigned to "<< realOperand->getName();
		if (realOperand->getValue() == TRUE)
			std::cout << " and " << realOperand->getName() << " is assigned in facts, so " << realOperand->getName() << " is " << statusToString(realOperand->getValue()) << std::endl;
		else
		{
			if (realOperand->getValue() == NOT_RESOLVED)
			{
				realOperand->setValue(FALSE);
			}
			std::cout << " so " << realOperand->getName() << " default value is " << statusToString(realOperand->getValue()) << std::endl;
		}
		return TRUE;
	}

	t_status result = realOperand->getIsResolved() ? realOperand->getValue() : NOT_RESOLVED;
	t_status tmp_result = NOT_RESOLVED;

	/*
	**	Resolve all rules related to querying operand
	*/
	for (size_t i = 0; i < consequents.size(); i++)
	{
//		std::cout << "Operand related : "<< realOperand->getName() << " in " << consequents[i].getToString();
		tmp_result = resolveRule(consequents[i], path, *realOperand);
//		std::cout << "results for operand : " << statusToString(result) << " && " << statusToString(tmp_result) << std::endl;
		if (result == NOT_RESOLVED)
		{
			result = tmp_result;
		}
		else if (tmp_result == NOT_RESOLVED)
		{
			tmp_result = result;
		}
		if (realOperand->getValue() == ERROR)
		{
			std::cout << std::endl;
			for (int i_tabs = 0; i_tabs < tabs; i_tabs++)
				std::cout << "    ";
			std::cout << realOperand->getName() << " cannot have 2 values." << std::endl;
			return ERROR;
		}
//		else if (result != UNDEFINED && result != tmp_result)
//		{
//			std::cout << std::endl;
//			for (int i_tabs = 0; i_tabs < tabs; i_tabs++)
//				std::cout << "    ";
//			std::cout << "2 results for operand : " << statusToString(result) << " != " << statusToString(tmp_result) << std::endl;
//			return ERROR;
//		}
	}

	std::cout << realOperand->getName() << " is " << statusToString(realOperand->getValue()) << std::endl;
	return TRUE;
}

void                ExpertSystem::resolveAllQueries()
{
	printRules();
	/*
	**  Call resolveQuery() on each query.
	*/
	t_status					result = NOT_RESOLVED;
	std::vector<Operand *>		path;
	std::stringstream			ss;
	std::vector<std::string>	results;

	while (!_queries.empty())
	{
		char c = _queries.front()->getName();
		std::cout << std::endl <<  "Query name : " << c << std::endl;
		result = resolveQuery(*popQuery(), path);
		if (result == ERROR)
		{
			ss.str("");
			path.clear();
			results.clear();
			ss << "The fact \"" << c << "\" cannot be resolved.";
			throw CannotBeResolvedException(ss.str());
		}
		else
		{
			ss << "Query name : " << c << ", result : " << statusToString(findOperand(c)->getValue());
		}
		results.push_back(ss.str());
		ss.str("");
		path.clear();
	}

	for (size_t i = 0; i < results.size(); i++)
	{
		std::cout << results[i] << std::endl;
	}

	results.clear();
}

/*
**	Debug
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
		printRule(_rules[i]);
	}
}

void				ExpertSystem::printRule(Rule r)
{
		printTokenList(r.getAllAntecedents(), false);
		
		std::cout << printLexemValue(r.getImplying()) << " ";
		printTokenList(r.getAllConsequents(), true);
}

/*
**	ExpertSystem::exception
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

