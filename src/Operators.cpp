#include "expertSystem.hpp"

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

    std::cout << "or operator" << std::endl;
	if (first.getOperand()->getValue() == first_value || second.getOperand()->getValue() == second_value)
    {
    std::cout << "true" << std::endl;
		return TRUE;
    }
	else 
	{
    std::cout << "false" << std::endl;
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
