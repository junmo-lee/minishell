#include "parser.h"

int	atoi_check_num(const char *str, int *target)
{
	long long	ret;
	int			sign;

	sign = 1;
	ret = 0;
	while (*str != '\0' && *str == ' ')
		str++;
	if (*str == '-' && ('0' <= *(str + 1) && *(str + 1) <= '9'))
	{
		sign = -1;
		str++;
	}
	else if (*str == '+' && ('0' <= *(str + 1) && *(str + 1) <= '9'))
		str++;
	while (*str != '\0' && *str != ' ')
	{
		if (*str < '0' || *str > '9')
			return (0);
		ret *= 10;
		ret += *str - '0';
		str++;
	}
	*target = sign * ret;
	return (1);
}