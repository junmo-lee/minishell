#include "../parser.h"

int	builtin_exit(t_parser_list *head_next, t_status *status)
{
	int	exit_val;

	if (head_next == NULL)
	{
		//ft_putstr_fd("exit\n", STDOUT_FILENO);
		exit(EXIT_SUCCESS);
	}
	if (atoi_check_num(head_next->token, &exit_val)) // 첫번째가 숫자인지 확인 + 숫자를 넣음
	{
		if (head_next->next == NULL) // 두번째 넘어가 존재하는지 확인
			exit((unsigned char)(exit_val)); // 존재하지 않는다면 그 숫자로 리턴
		else
		{
			//ft_putstr_fd("exit\nexit: too many arguments", STDERR_FILENO);
			status->exit_status = 1; // (exit \n bash: exit: too many arguments) exit는 되면 안됨, 
		}
	}
	else // 처음 오는 게 숫자가 아닐때
	{
		//ft_putstr_fd("exit\nexit: a: numeric argument required", STDERR_FILENO);
		exit(255); //exit\n bash: exit: a: numeric argument required
	}
	return (0);
}