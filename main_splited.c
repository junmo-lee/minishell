#include "parser.h"

void	count_parsed_tree(t_parsed_tree *head)
{
	t_parsed_tree	*current_node;
	t_parser_list	*parser_node;

	current_node = head;
	while (current_node != NULL)
	{
		current_node->arg_len = 0;
		parser_node = current_node->cmd_list_head;
		while (parser_node != NULL)
		{
			current_node->arg_len++;
			parser_node = parser_node->next;
		}
		current_node = current_node->next;
	}
}

void	main_status_init(t_status *status, t_vars *vars, \
t_envp_list **envp_list, char **envp)
{
	ft_memset(vars, 0, sizeof(vars));
	ft_memset(status, 0, sizeof(status));
	status->one_line = vars;
	status->env_list = get_envp(envp);
	*envp_list = status->env_list;
	if (getcwd(status->pwd, PATH_MAX) == NULL)
		exit(EXIT_FAILURE);
	insert_envp_node(envp_list, ft_strdup("OLDPWD"), NULL);
}

void	tree_no_error(t_status *status, t_vars *vars, \
t_parsed_tree *head, t_envp_list *envp_list)
{
	count_parsed_tree(head);
	g_signal = 0;
	status->exit_status = 0;
	status->env_list = envp_list;
	status->exit_status = run_cmd_tree(status, head, vars);
	if (vars->is_here_doc == 1)
	{
		unlink(vars->temp_here_doc);
		free(vars->temp_here_doc);
	}
	if (vars->cmd_len == 1)
	{
		if (ft_strncmp(head->cmd_list_head->token, "export", 7) == 0)
			export(head->cmd_list_head, &envp_list);
		else if (ft_strncmp(head->cmd_list_head->token, "unset", 6) == 0)
			unset(head->cmd_list_head, &envp_list);
		else if (ft_strncmp(head->cmd_list_head->token, "cd", 3) == 0)
			cd(head->cmd_list_head, &envp_list, status->pwd);
		else if (ft_strncmp(head->cmd_list_head->token, "exit", 5) == 0)
			builtin_exit(head->cmd_list_head->next, status);
	}
}

int	str_exist(char *str, t_status *status, t_vars *vars, t_envp_list *envp_list)
{
	t_parsed_tree	*head;

	add_history(str);
	head = parser(str, status, envp_list);
	bubble_sort(envp_list);
	vars_init(status->one_line, envp_list);
	if (head->error == NO_ERROR)
		tree_no_error(status, vars, head, envp_list);
	else
	{
		if (head->error == NOT_CLOSED_ERROR)
			ft_putstr_fd("NOT_CLOSED_ERROR\n", STDERR_FILENO);
		else if (head->error == MALLOC_ERROR)
			ft_putstr_fd("MALLOC_ERROR\n", STDERR_FILENO);
		else if (head->error == REDIRECTION_ERROR)
			ft_putstr_fd("REDIRECTION_ERROR\n", STDERR_FILENO);
		else if (head->error == PIPE_ERROR)
			ft_putstr_fd("PIPE_ERROR\n", STDERR_FILENO);
	}
	if (vars->path != NULL)
		free_strs(vars->path, EXIT_SUCCESS);
	if (vars->envp != NULL)
		free_strs(vars->envp, EXIT_SUCCESS);
	clear_parsed_tree(&head);
	return (EXIT_SUCCESS);
}
