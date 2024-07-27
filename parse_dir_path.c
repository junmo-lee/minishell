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

void	update_pwd(char *pwd, const char *str)
{
	char	**temp_list;
	char	**pwd_part;
	int		pwd_part_len;
	int		depth;
	char	**str_part;
	int		len;
	int		i;

	if (pwd == NULL || str == NULL)
		return ;
	// cd 로 인해 pwd 가 업데이트 되는것이기 때문에 확인해야함
	if (access(str, X_OK) != 0)
		return ;
	pwd_part = ft_split(pwd, '/');
	str_part = ft_split(str, '/');
	if (pwd_part == NULL || str_part == NULL)
		exit(EXIT_FAILURE);
	pwd_part_len = strs_len(pwd_part);
	temp_list = malloc(sizeof(char *) * (pwd_part_len + strs_len(str_part) + 1));
	if (temp_list == NULL)
		exit(EXIT_FAILURE);

	if (str[0] != '/')
	{
		i = 0;
		// 상대 경로일때 현재 pwd 를 앞에 넣어줌
		while (pwd_part[i] != NULL)
		{
			temp_list[i] = pwd_part[i];
			// fprintf(stderr, "old_pwd : [%s]\n", temp_list[i]);
			i++;
		}
		depth = i;
	}
	else
	{
		depth = 0;
	}

	i = -1;
	while (str_part[++i] != NULL)
	{
		if (str_part[i][0] == '\0' || ft_strncmp(str_part[i], ".", 2) == 0)
			continue;
		else if (ft_strncmp(str_part[i], "..", 3) == 0)
		{
			depth--;
		}
		else
		{
			temp_list[depth] = str_part[i];
			depth++;
		}
	}

	if (depth < 0)
		exit(EXIT_FAILURE);

	ft_memset(pwd, 0, ft_strlen(pwd));
	i = 0;
	while (i < depth)
	{
		len = ft_strlen(pwd);
		pwd[len] = '/';
		pwd[len + 1] = '\0';
		ft_strlcat(pwd, temp_list[i], ft_strlen(pwd) + ft_strlen(temp_list[i]) + 1);
		//ft_strlcat 으로 1024 버퍼크기 넘어간거 확인?
		i++;
	}

	free(temp_list);
	free_strs(pwd_part, EXIT_SUCCESS);
	free_strs(str_part, EXIT_SUCCESS);

}