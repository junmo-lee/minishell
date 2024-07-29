#include "../parser.h"
// t_parser_list *cmd_list, t_envp_list **envp_list
// cd(head->cmd_list_head, status.pwd);
int cd(t_parser_list *cmd_list, t_envp_list **envp_list, char *pwd)
{
	if (cmd_list->next == NULL)
		return (0);
	if (cmd_list->next->token == NULL)
		return (0);
	if (access(cmd_list->next->token, X_OK) == 0)
	{
		insert_envp_node(envp_list, ft_strdup("OLDPWD"), pwd);
		update_pwd(pwd, cmd_list->next->token);
		insert_envp_node(envp_list, ft_strdup("PWD"), pwd);
		(void)envp_list;
		chdir(pwd);
	}
	return (0);
}