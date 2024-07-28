#include "pipex_bonus.h"
#include "../parser.h"
#include <signal.h>
// int	get_signal_code(int x)
// {
// 	// _W_INT(w)       (*(int *)&(w))
// 	// _WSTATUS(x)     (_W_INT(x) & 0177)
// 	// _WSTOPPED       0177
// 	// _WSTATUS(x)     (_W_INT(x) & 0177)
//  	return (_WSTATUS(x) != _WSTOPPED && _WSTATUS(x) != 0);
// }

void	sig_here_doc(int signo)
{
	//fprintf(stderr, "sig_here_doc\n");
	if (signo == SIGINT)
	{
		write(STDERR_FILENO, "\n", 1);
	}
}

void	sig_h(int signo)
{
	//fprintf(stderr, "sig_h\n");
	g_signal = HERE_DOC_SIGINT;
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	exit(signo);
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
	vars->is_here_doc = 1;
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
	// here_doc 을 stdin 에서 받아오는 과정을 새로운 프로세스에서
	fork_ret = fork();
	if (fork_ret == 0)
	{
		signal(SIGINT, sig_h);
		signal(SIGQUIT, SIG_IGN);
		write_file(&(vars->fd_here_doc), vars->temp_here_doc, \
			O_WRONLY | O_CREAT | O_TRUNC);
		fprintf(stderr, "<< [%s]\n", token);
		write_here_doc(vars->fd_here_doc, token);
		close(vars->fd_here_doc);
		exit(EXIT_SUCCESS);
	}
	else
	{
		signal(SIGINT, sig_here_doc);
		signal(SIGQUIT, SIG_IGN);
		waitpid(fork_ret, &process_status, 0);
		//waitpid(fork_ret, &process_status, 0);
		//fprintf(stderr, "wait end!\n");
		// kill(fork_ret, SIGINT);
		// signal(SIGINT, signal_handler);
		// signal(SIGQUIT, stdin_handler);
		if (WIFSIGNALED(process_status))
		{
		// if (g_signal == HERE_DOC_SIGINT)
		// {
			fprintf(stderr, "heredoc process exit with signal : %d\n", WTERMSIG(process_status));
			return (WTERMSIG(process_status));
		}
		else
		{
			read_file(&(cmd->redirection_in), vars->temp_here_doc, O_RDONLY);
			return (0);
		}
	}
}