#include "pipex_bonus.h"
#include "../parser.h"
#include <signal.h>









void	sig_heredoc(int signo)
{
	(void)signo;
	write(STDOUT_FILENO, "\n", 2);
}

int	make_here_doc(t_vars *vars, t_cmd *cmd, char *token)
{
	char	*temp_dir = NULL;
	char	*temp_number = NULL;
	int		number = 0;
	int		process_status;
	pid_t	fork_ret;

	if (vars->is_here_doc == 1)
	{
		unlink(vars->temp_here_doc);
		free(vars->temp_here_doc);
	}
	while(1)
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
	
	fork_ret = fork();
	if (fork_ret == 0)
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
	else
	{
		rl_catch_signals = 0;
		signal(SIGINT, sig_heredoc);
		signal(SIGQUIT, SIG_IGN);
		waitpid(fork_ret, &process_status, 0);
		signal(SIGINT, stdin_handler);
		signal(SIGQUIT, stdin_handler);
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
}