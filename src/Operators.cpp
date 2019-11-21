#include "expertSystem.hpp"

/*
**	Debug
*/
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

/*
**	if token (!= operand) is negative
*/
t_status		getReverseValue(t_status value)
{
	return (value == NOT_RESOLVED) ? TRUE :
           (value == ERROR) ? ERROR:
           (value == UNDEFINED) ? UNDEFINED :
           FALSE;
}

/*
**	operators in equation
*/
t_status		andOperator(Token const &first, Token const &second)
{
	if (first.getOperand()->getValue() == ERROR || second.getOperand()->getValue() == ERROR)
		return ERROR;
//	t_status tmp = getReverseValue(first.getOperand()->getValue());
//	t_status first_value = first.getIsNegativeOperand() ? tmp : first.getOperand()->getValue();
//
//	tmp = getReverseValue(second.getOperand()->getValue());
//	t_status second_value = second.getIsNegativeOperand() ? tmp : second.getOperand()->getValue();

	if (first.getOperand()->getValue() == UNDEFINED || second.getOperand()->getValue() == UNDEFINED)
		return UNDEFINED;
	if (first.getOperand()->getValue() == TRUE && second.getOperand()->getValue() == TRUE)
	{
		return TRUE;
	}
	else 
	{
		return FALSE;
	}
}

t_status		orOperator(Token const &first, Token const &second)
{
	bool first_value = first.getIsNegativeOperand() ? false: true;
	bool second_value = second.getIsNegativeOperand() ? false: true;

	if (first.getOperand()->getValue() == ERROR || second.getOperand()->getValue() == ERROR)
		return ERROR;
	if ((first.getOperand()->getValue() == UNDEFINED && second.getOperand()->getValue() == second_value) ||
		(second.getOperand()->getValue() == UNDEFINED && first.getOperand()->getValue() == first_value) ||
	(first.getOperand()->getValue() == first_value || second.getOperand()->getValue() == second_value))
    {
		return TRUE;
    }
	else if (first.getOperand()->getValue() == UNDEFINED || second.getOperand()->getValue() == UNDEFINED)
		return UNDEFINED;
	else 
	{
        return FALSE;
    }
}

t_status		xorOperator(Token const &first, Token const &second)
{
	bool first_value = first.getIsNegativeOperand() ? false: true;
	bool second_value = second.getIsNegativeOperand() ? false: true;

	if (first.getOperand()->getValue() == ERROR || second.getOperand()->getValue() == ERROR)
		return ERROR;
	if (first.getOperand()->getValue() == UNDEFINED || second.getOperand()->getValue() == UNDEFINED)
		return ERROR;
	if (first.getOperand()->getValue() == first_value && second.getOperand()->getValue() != second_value)
		return TRUE;
	if (first.getOperand()->getValue() != first_value && second.getOperand()->getValue() == second_value)
		return TRUE;
	else 
		return FALSE;
}

/*
**	assign value to operand in result
*/
t_status        assignAnd(Token const &first, Token const &second, t_status value)
{
	
    t_status reverse_value = getReverseValue(value);

    t_status tmp = first.getIsNegativeOperand() ? reverse_value : value;

//std::cout << "assignAnd()" << std::endl;
	//printStatus(value);
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

//	std::cout << "assignOr()" << std::endl;
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

void            assignValue(Token const &token, t_status value)
{
    t_status reverse_value = getReverseValue(value);

    t_status tmp = token.getIsNegativeOperand() ? reverse_value : value;
//	std::cout << "assignValue()" << std::endl;
//	std::cout << "tmp : " << statusToString(tmp)<< std::endl;
    if (token.getOperand()->getIsResolved() && token.getOperand()->getValue() != tmp)
        token.getOperand()->setValue(ERROR);
    else
        token.getOperand()->setValue(tmp);
    token.getOperand()->setIsResolved(true);
}
