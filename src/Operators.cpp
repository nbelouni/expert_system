#include "expertSystem.hpp"

t_status		getReverseValue(t_status value)
{
	return (value == NOT_RESOLVED) ? TRUE :
           (value == FALSE) ? TRUE:
           (value == TRUE) ? FALSE:
           (value == UNDEFINED) ? UNDEFINED :
           FALSE;
}
void            printStatus(t_status result)
{
	std::cout << (result == NOT_RESOLVED ?	"NOT_RESOLVED" :
				 result == UNDEFINED ?		"UNDEFINED" :
				 result == TRUE ?			"TRUE" :
				 result == FALSE ?			"FALSE" :
											"ERROR") << std::endl;
}

const char *            statusToString(t_status result)
{
	return (result == NOT_RESOLVED ?	"NOT_RESOLVED" :
				 result == UNDEFINED ?		"UNDEFINED" :
				 result == TRUE ?			"TRUE" :
				 result == FALSE ?			"FALSE" :
											"ERROR");
}

t_status		andOperator(Token const &first, Token const &second)
{
	bool first_value = first.getIsNegativeOperand() ? false : true;
	bool second_value = second.getIsNegativeOperand() ? false : true;

	if (first.getOperand()->getValue() == first_value && second.getOperand()->getValue() == second_value)
		return TRUE;
	else 
		return FALSE;
}

t_status		orOperator(Token const &first, Token const &second)
{
	bool first_value = first.getIsNegativeOperand() ? false: true;
	bool second_value = second.getIsNegativeOperand() ? false: true;

    //std::cout << "or operator" << std::endl;
	if (first.getOperand()->getValue() == UNDEFINED && second.getOperand()->getValue() == UNDEFINED)
		return UNDEFINED;
	else if (first.getOperand()->getValue() == first_value || second.getOperand()->getValue() == second_value)
    {
    //std::cout << "true" << std::endl;
		return TRUE;
    }
	else 
	{
    //std::cout << "false" << std::endl;
        return FALSE;
    }
}

t_status		xorOperator(Token const &first, Token const &second)
{
	bool first_value = first.getIsNegativeOperand() ? false: true;
	bool second_value = second.getIsNegativeOperand() ? false: true;

	if (first.getOperand()->getValue() == first_value && second.getOperand()->getValue() == (!second_value))
		return TRUE;
	if (first.getOperand()->getValue() == (!first_value) && second.getOperand()->getValue() == second_value)
		return TRUE;
	else 
		return FALSE;
}

t_status        assignAnd(Token const &first, Token const &second, t_status value)
{
    t_status reverse_value = getReverseValue(value);

    t_status tmp = first.getIsNegativeOperand() ? reverse_value : value;

	//std::cout << "assignAnd()" << std::endl;
	printStatus(value);
	//std::cout << first.getOperand()->getName() << " : " << statusToString(first.getOperand()->getValue()) << " " << (first.getOperand()->getIsResolved() ? "true" : "false") << std::endl;
	//std::cout << second.getOperand()->getName() << " : " << statusToString(second.getOperand()->getValue()) << " " << (second.getOperand()->getIsResolved() ? "true" : "false") << std::endl;
    if (first.getOperand()->getIsResolved() && first.getOperand()->getValue() != tmp && first.getOperand()->getValue() != UNDEFINED)
        return NOT_RESOLVED;

    first.getOperand()->setValue(tmp);
    first.getOperand()->setIsResolved(true);

    tmp = second.getIsNegativeOperand() ? reverse_value : value;

	//std::cout << first.getOperand()->getName() << " : " << statusToString(first.getOperand()->getValue()) << " " << (first.getOperand()->getIsResolved() ? "true" : "false") << std::endl;
	//std::cout << second.getOperand()->getName() << " : " << statusToString(second.getOperand()->getValue()) << " " << (second.getOperand()->getIsResolved() ? "true" : "false") << std::endl;
    if (second.getOperand()->getIsResolved() && second.getOperand()->getValue() != tmp && second.getOperand()->getValue() != UNDEFINED)
        return NOT_RESOLVED;

    second.getOperand()->setValue(second.getIsNegativeOperand() ? reverse_value : value);
    second.getOperand()->setIsResolved(true);
	//std::cout << "assignAnd()" << std::endl;
	//std::cout << first.getOperand()->getName() << " : " << statusToString(first.getOperand()->getValue()) << " " << (first.getOperand()->getIsResolved() ? "true" : "false") << std::endl;
	//std::cout << second.getOperand()->getName() << " : " << statusToString(second.getOperand()->getValue()) << " " << (second.getOperand()->getIsResolved() ? "true" : "false") << std::endl;
    return andOperator(first, second);
}

t_status        assignOr(Token const &first, Token const &second, t_status value)
{
    t_status reverse_value = getReverseValue(value);

    t_status tmp = first.getIsNegativeOperand() ? reverse_value : value;

//	//std::cout << "assignOr()" << std::endl;
//	//std::cout << first.getOperand()->getName() << " : " << statusToString(first.getOperand()->getValue()) << " " << (first.getOperand()->getIsResolved() ? "true" : "false") << std::endl;
//	//std::cout << second.getOperand()->getName() << " : " << statusToString(second.getOperand()->getValue()) << " " << (second.getOperand()->getIsResolved() ? "true" : "false") << std::endl;
    if (!first.getOperand()->getIsResolved())
    {
        first.getOperand()->setValue(UNDEFINED);
        first.getOperand()->setIsResolved(true);
    }
    else if (first.getOperand()->getIsResolved() && first.getOperand()->getValue() != UNDEFINED && first.getOperand()->getValue() != tmp)
        return NOT_RESOLVED;
		tmp = second.getIsNegativeOperand() ? reverse_value : value;

		if (!second.getOperand()->getIsResolved())
		{
			second.getOperand()->setValue(UNDEFINED);
			second.getOperand()->setIsResolved(true);
		}
    	else if (second.getOperand()->getIsResolved() && second.getOperand()->getValue() != UNDEFINED && second.getOperand()->getValue() != tmp)
        	return NOT_RESOLVED;

//	//std::cout << first.getOperand()->getName() << " : " << statusToString(first.getOperand()->getValue()) << " " << (first.getOperand()->getIsResolved() ? "true" : "false") << std::endl;
//	//std::cout << second.getOperand()->getName() << " : " << statusToString(second.getOperand()->getValue()) << " " << (second.getOperand()->getIsResolved() ? "true" : "false") << std::endl;
    return orOperator(first, second);
}

t_status        assignXor(Token const &first, Token const &second, t_status value)
{
    t_status reverse_value = getReverseValue(value);

    t_status tmp = first.getIsNegativeOperand() ? reverse_value : value;

	std::cout << "assignXOR()" << std::endl;
	printStatus(value);
	std::cout << first.getOperand()->getName() << " : " << statusToString(first.getOperand()->getValue()) << " " << (first.getOperand()->getIsResolved() ? "true" : "false") << std::endl;
	std::cout << second.getOperand()->getName() << " : " << statusToString(second.getOperand()->getValue()) << " " << (second.getOperand()->getIsResolved() ? "true" : "false") << std::endl;
	if (!first.getOperand()->getIsResolved() && !second.getOperand()->getIsResolved())
        return NOT_RESOLVED;
    if (first.getOperand()->getIsResolved() && first.getOperand()->getValue() != tmp)
        return NOT_RESOLVED;
    tmp = second.getIsNegativeOperand() ? reverse_value : value;
    if (second.getOperand()->getIsResolved() && second.getOperand()->getValue() != tmp)
        return NOT_RESOLVED;

	if (second.getOperand()->getIsResolved() && !first.getOperand()->getIsResolved())
	{
    	first.getOperand()->setValue(getReverseValue(second.getOperand()->getValue()));
    	first.getOperand()->setIsResolved(true);
	}
	else if (first.getOperand()->getIsResolved() && !second.getOperand()->getIsResolved())
	{
    	second.getOperand()->setValue(getReverseValue(first.getOperand()->getValue()));
    	second.getOperand()->setIsResolved(true);
	}

	std::cout << first.getOperand()->getName() << " : " << statusToString(first.getOperand()->getValue()) << " " << (first.getOperand()->getIsResolved() ? "true" : "false") << std::endl;
	std::cout << second.getOperand()->getName() << " : " << statusToString(second.getOperand()->getValue()) << " " << (second.getOperand()->getIsResolved() ? "true" : "false") << std::endl;

	std::cout << first.getOperand()->getName() << " : " << statusToString(first.getOperand()->getValue()) << " " << (first.getOperand()->getIsResolved() ? "true" : "false") << std::endl;
	std::cout << second.getOperand()->getName() << " : " << statusToString(second.getOperand()->getValue()) << " " << (second.getOperand()->getIsResolved() ? "true" : "false") << std::endl;
    return xorOperator(first, second);
}

void            assignValue(Token const &token, t_status value)
{
    t_status reverse_value = (value == NOT_RESOLVED) ? TRUE :
                             (value == FALSE) ? TRUE:
                             (value == TRUE) ? FALSE:
                             (value == UNDEFINED) ? UNDEFINED :
                             FALSE;

    t_status tmp = token.getIsNegativeOperand() ? reverse_value : value;
    if (token.getOperand()->getIsResolved() && token.getOperand()->getValue() != tmp)
        token.getOperand()->setValue(ERROR);
    else
        token.getOperand()->setValue(tmp);
    token.getOperand()->setIsResolved(true);
}
