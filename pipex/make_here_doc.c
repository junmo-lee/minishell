#include "../parser.h"

void	sig_heredoc(int signo)
{
	(void)signo;
	write(STDOUT_FILENO, "\n", 2);
}

void	make_here_doc_name(t_vars *vars)
{
	char	*temp_dir;
	char	*temp_number;
	int		number;

	temp_dir = NULL;
	temp_number = NULL;
	number = 0;
	if (vars->is_here_doc == 1)
	{
		unlink(vars->temp_here_doc);
		free(vars->temp_here_doc);
	}
	while (1)
	{
		temp_number = ft_itoa(number);
		temp_dir = ft_strjoin("/tmp/minishell_heredoc",temp_number);
		free(temp_number);
		if (access(temp_dir, F_OK) != 0)
		{
			vars->temp_here_doc = temp_dir;
			break ;
		}
		free(temp_dir);
		number++;
	}
}

void	make_here_doc_process(t_vars *vars, char *token)
{
	rl_catch_signals = 1;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	write_file(&(vars->fd_here_doc), vars->temp_here_doc, \
		O_WRONLY | O_CREAT | O_TRUNC);
	write_here_doc(vars->fd_here_doc, token);
	close(vars->fd_here_doc);
	exit(EXIT_SUCCESS);
}

void	wait_here_doc_process(pid_t fork_ret, int *process_status)
{
	rl_catch_signals = 0;
	signal(SIGINT, sig_heredoc);
	signal(SIGQUIT, SIG_IGN);
	waitpid(fork_ret, process_status, 0);
	signal(SIGINT, stdin_handler);
	signal(SIGQUIT, stdin_handler);
}

int	make_here_doc(t_vars *vars, t_cmd *cmd, char *token)
{
	int		process_status;
	pid_t	fork_ret;

	make_here_doc_name(vars);
	fork_ret = fork();
	if (fork_ret == 0)
		make_here_doc_process(vars, token);
	else
	{
		wait_here_doc_process(fork_ret, &process_status);
		if (WIFSIGNALED(process_status))
		{
			unlink(vars->temp_here_doc);
			return (WTERMSIG(process_status));
		}
		else
		{
			vars->is_here_doc = 1;
			read_file(&(cmd->redirection_in), vars->temp_here_doc, O_RDONLY);
			return (0);
		}
	}
	return (0);
}
