/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmlee   <junmlee@student.42seoul.k>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 15:19:33 by junmlee           #+#    #+#             */
/*   Updated: 2024/08/01 21:08:57 by junmlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	leaks_check(void)
{
	system("leaks minishell");
}

void	signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	if (signo == SIGQUIT)
	{
		rl_on_new_line();
		rl_redisplay();
	}
}

int	do_loop(t_status *status, t_vars *vars, t_envp_list *envp_list)
{
	char	*str;

	while (1)
	{
		str = NULL;
		rl_catch_signals = 0;
		signal(SIGINT, signal_handler);
		signal(SIGQUIT, SIG_IGN);
		str = readline("prompt : \001\033[s\002");
		if (str == NULL)
		{
			write(STDOUT_FILENO, "\033[u\033[1B\033[1A", 12);
			write(STDOUT_FILENO, "exit\n", sizeof("exit\n"));
			break ;
		}
		if (str[0] != '\0')
			str_exist(str, status, vars, envp_list);
		free(str);
	}
	clear_envp_list(&envp_list);
	return (EXIT_SUCCESS);
}

int	prev_terminal_setting(t_status *status, \
t_vars *vars, t_envp_list *envp_list)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	return (do_loop(status, vars, envp_list));
}

int	main(int argc, char **argv, char **envp)
{
	t_status		status;
	t_vars			vars;
	t_envp_list		*envp_list;

	atexit(leaks_check);
	(void)argv;
	if (argc != 1)
	{
		ft_putstr_fd("argument input error", STDERR_FILENO);
		return (0);
	}
	main_status_init(&status, &vars, &envp_list, envp);
	return (prev_terminal_setting(&status, &vars, envp_list));
}
