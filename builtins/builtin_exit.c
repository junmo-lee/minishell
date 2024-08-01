#include "../minishell.h"

int	builtin_exit(t_parser_list *head_next, t_status *status)
{
	int	exit_val;

	if (head_next == NULL)
		exit(EXIT_SUCCESS);
	if (atoi_check_num(head_next->token, &exit_val))
	{
		if (head_next->next == NULL)
			exit((unsigned char)(exit_val));
		else
			status->exit_status = 1;
	}
	else
		exit(EXIT_NUMERIC);
	return (0);
}
