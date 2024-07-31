#include "parser.h"

int	strs_len(char **strs)
{
	int	len;

	if (strs == NULL)
		return (0);
	len = 0;
	while (strs[len] != NULL)
		len++;
	return (len);
}

char	**make_part(char ***tpwd, char *pwd, char ***tstr, char *str)
{
	char	**ret;
	int		i;

	*tpwd = ft_split(pwd, '/');
	*tstr = ft_split(str, '/');
	if (*tpwd == NULL || *tstr == NULL)
		exit(EXIT_FAILURE);
	ret = malloc(sizeof(char *) * (strs_len(*tpwd) + strs_len(*tstr) + 1));
	if (ret == NULL)
		exit(EXIT_FAILURE);
	ft_memset(ret, 0, sizeof(ret));
	i = 0;
	if (str[0] != '/')
	{
		while ((*tpwd)[i] != NULL)
		{
			ret[i] = ft_strdup((*tpwd)[i]);
			i++;
		}
	}
	ret[i] = NULL;
	return (ret);
}

char	**parse_dir(char *pwd, char *str)
{
	char	**ret;
	char	**pwd_part;
	char	**str_part;
	int		depth;
	int		i;

	ret = make_part(&pwd_part, pwd, &str_part, str);
	depth = strs_len(ret);
	i = -1;
	while (str_part[++i] != NULL)
	{
		if (depth < 0)
			break ;
		if (str_part[i][0] == '\0' || ft_strncmp(str_part[i], ".", 2) == 0)
			continue ;
		else if (ft_strncmp(str_part[i], "..", 3) == 0)
			free(ret[depth--]);
		else
			ret[depth++] = ft_strdup(str_part[i]);
	}
	ret[depth + 1] = NULL;
	free_strs(pwd_part, EXIT_SUCCESS);
	free_strs(str_part, EXIT_SUCCESS);
	return (ret);
}

void	pwd_cat_temp_list(char *pwd, char **temp_list, int depth)
{
	int	i;
	int	len;

	i = 0;
	while (i < depth)
	{
		len = ft_strlen(pwd);
		pwd[len] = '/';
		pwd[len + 1] = '\0';
		ft_strlcat(pwd, temp_list[i], \
			ft_strlen(pwd) + ft_strlen(temp_list[i]) + 1);
		i++;
	}
}

void	update_pwd(char *pwd, char *str)
{
	char	**temp_list;
	int		depth;

	if (pwd == NULL || str == NULL)
		return ;
	if (access(str, X_OK) != 0)
		return ;
	temp_list = parse_dir(pwd, str);
	depth = strs_len(temp_list);
	ft_memset(pwd, 0, ft_strlen(pwd));
	if (temp_list[0] == NULL)
	{
		free(temp_list);
		pwd[0] = '/';
	}
	else
	{
		pwd_cat_temp_list(pwd, temp_list, depth);
		free_strs(temp_list, EXIT_SUCCESS);
	}
}
