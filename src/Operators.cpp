#include "expertSystem.hpp"

t_status		andOperator(t_status const first, t_status const second)
{
	std::cout << "andOperator" << std::endl;
	if (first != NOT_RESOLVED && second != NOT_RESOLVED)
	{
		if (first == TRUE && second == TRUE)
			return TRUE;
		else
			return FALSE;
	}
	return (NOT_RESOLVED);
}

t_status		orOperator(t_status const first, t_status const second)
{
	std::cout << "orOperator" << std::endl;
	if (first != NOT_RESOLVED && second != NOT_RESOLVED)
	{
		if (first == TRUE || second == TRUE)
			return TRUE;
		else 
			return FALSE;
	}
	return (NOT_RESOLVED);
}

t_status		xorOperator(t_status const first, t_status const second)
{
	std::cout << "xorOperator" << std::endl;
	if (first != NOT_RESOLVED && second != NOT_RESOLVED)
	{
		if (first == TRUE && second != TRUE)
			return TRUE;
		if (first != TRUE && second == TRUE)
			return TRUE;
		else 
			return FALSE;
	}
	return (NOT_RESOLVED);
}
