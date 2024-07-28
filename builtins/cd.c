#include "../parser.h"
// t_parser_list *cmd_list, t_envp_list **envp_list
// cd(head->cmd_list_head, status.pwd);
int cd(t_parser_list *cmd_list, t_envp_list **envp_list, char *pwd)
{
	if (cmd_list->next == NULL)
		return (0);
	if (cmd_list->next->token == NULL)
		return (0);
	fprintf(stderr, "cd : %s\narg : %s\npwd : %s\n", cmd_list->token, cmd_list->next->token, pwd);
	if (access(cmd_list->next->token, X_OK) == 0)
	{
		update_pwd(pwd, cmd_list->next->token);
		(void)envp_list;
		chdir(pwd);
	}
	return (0);
}