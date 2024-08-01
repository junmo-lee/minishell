#include "../minishell.h"

void	print_full_path(char *pwd, char *str)
{
	char	buf[PATH_MAX + 1];

	parse_dir(buf, pwd, str);
	write(STDERR_FILENO, buf, ft_strlen(buf));
	write_stderr_exit(NULL, ": Permission denied", EACCES_EXIT_CODE);
}
