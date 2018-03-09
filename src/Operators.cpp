#include "expertSystem.hpp"

t_status		andOperator(Operand const &first, Operand const &second)
{
	if (first.getIsResolved() == true && second.getIsResolved() == true)
	{
		if (first.getValue() == true && first.getValue() == true)
			return TRUE;
		else 
			return FALSE;
	}
	return (NOT_RESOLVED);
}

t_status		orOperator(Operand const &first, Operand const &second)
{
	if (first.getIsResolved() == true && second.getIsResolved() == true)
	{
		if (first.getValue() == true || first.getValue() == true)
			return TRUE;
		else 
			return FALSE;
	}
	return (NOT_RESOLVED);
}

t_status		xorOperator(Operand const &first, Operand const &second)
{
	if (first.getIsResolved() == true && second.getIsResolved() == true)
	{
		if (first.getValue() == true && first.getValue() == false)
			return TRUE;
		if (first.getValue() == false && first.getValue() == true)
			return TRUE;
		else 
			return FALSE;
	}
	return (NOT_RESOLVED);
}
