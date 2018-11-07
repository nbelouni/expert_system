#include "expertSystem.hpp"

void			printStatus(t_status result)
{
	std::cout << (result == NOT_RESOLVED ?	"NOT_RESOLVED" :
				 result == UNDEFINED ?		"UNDEFINED" :
				 result == TRUE ?			"TRUE" :
				 result == FALSE ?			"FALSE" :
											"ERROR") << std::endl;
}

const char *			statusToString(t_status result)
{
	return (result == NOT_RESOLVED ?	"NOT_RESOLVED" :
				 result == UNDEFINED ?		"UNDEFINED" :
				 result == TRUE ?			"TRUE" :
				 result == FALSE ?			"FALSE" :
											"ERROR");
}

t_status		setReverseValue(t_status value)
{
	if (value == NOT_RESOLVED || value == FALSE)
    {
		return TRUE;
    }
	else if (value == UNDEFINED)
	{
        return UNDEFINED;
    }
	return FALSE;
}

t_status		andOperator(Token const &first, Token const &second)
{
	bool first_value = first.getIsNegativeOperand() ? false : true;
	bool second_value = second.getIsNegativeOperand() ? false : true;

	if (first.getOperand()->getValue() == first_value &&
        second.getOperand()->getValue() == second_value)
    {
		return TRUE;
    }
	return FALSE;
}

t_status		orOperator(Token const &first, Token const &second)
{
	bool first_value = first.getIsNegativeOperand() ? false: true;
	bool second_value = second.getIsNegativeOperand() ? false: true;

	if (first.getOperand()->getValue() == first_value ||
        second.getOperand()->getValue() == second_value)
	{
		return TRUE;
	}
	return FALSE;
}

t_status		xorOperator(Token const &first, Token const &second)
{
	bool first_value = first.getIsNegativeOperand() ? false: true;
	bool second_value = second.getIsNegativeOperand() ? false: true;

	if ((first.getOperand()->getValue() == first_value && second.getOperand()->getValue() == (!second_value)) ||
	    (first.getOperand()->getValue() == (!first_value) && second.getOperand()->getValue() == second_value))
    {
		return TRUE;
    }
    return FALSE;
}

t_status		assignAnd(Token const &first, Token const &second, t_status value)
{
	t_status reverse_value = setReverseValue(value);
	t_status tmp_1 = first.getIsNegativeOperand() ? reverse_value : value;
	t_status tmp_2 = second.getIsNegativeOperand() ? reverse_value : value;

	if ((first.getOperand()->getIsResolved() && second.getOperand()->getValue() != UNDEFINED && first.getOperand()->getValue() != tmp_1) ||
        (second.getOperand()->getIsResolved() && second.getOperand()->getValue() != UNDEFINED && second.getOperand()->getValue() != tmp_2))
    {
		return ERROR;
    }

	first.getOperand()->setValue(tmp_1);
	first.getOperand()->setIsResolved(true);

	second.getOperand()->setValue(second.getIsNegativeOperand() ? reverse_value : value);
	second.getOperand()->setIsResolved(true);

	return andOperator(first, second);
}

t_status		assignOr(Token const &first, Token const &second, t_status value)
{
	t_status reverse_value = setReverseValue(value);

	t_status tmp_1 = first.getIsNegativeOperand() ? reverse_value : value;
	t_status tmp_2 = second.getIsNegativeOperand() ? reverse_value : value;

	if ((first.getOperand()->getIsResolved() && first.getOperand()->getValue() != UNDEFINED && first.getOperand()->getValue() != tmp_1) ||
	(second.getOperand()->getIsResolved() && second.getOperand()->getValue() != UNDEFINED && second.getOperand()->getValue() != tmp_2))
    {
		return ERROR;
    }

	if (!first.getOperand()->getIsResolved())
	{
		first.getOperand()->setValue(UNDEFINED);
		first.getOperand()->setIsResolved(true);
	}


	if (!second.getOperand()->getIsResolved())
	{
		second.getOperand()->setValue(UNDEFINED);
		second.getOperand()->setIsResolved(true);
	}

	return orOperator(first, second);
}

void			assignValue(Token const &token, t_status value)
{
	t_status reverse_value = setReverseValue(value);
	t_status tmp = token.getIsNegativeOperand() ? reverse_value : value;

	if (token.getOperand()->getIsResolved() && token.getOperand()->getValue() != UNDEFINED && token.getOperand()->getValue() != tmp)
    {
		token.getOperand()->setValue(ERROR);
    }
	else
    {
		token.getOperand()->setValue(tmp);
    }

	token.getOperand()->setIsResolved(true);
}
