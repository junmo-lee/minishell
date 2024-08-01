#include "parser.h"

int	strs_len(char **strs)
{
	int	len;

	len = 0;
	while (strs[len] != NULL)
		len++;
	return (len);
}
// target(char [PATH_MAX]) 에 현재 pwd(char [PATH_MAX])와 움직일 경로 str 을 적용한 결과를 붙여넣음
// pwd를 update 시에는 overlap 이 일어나지 않도록 주의!
void	parse_dir(char *target, char *pwd, char *str)
{

	ft_memset(target, 0, PATH_MAX); // target을 초기화하고
	if (str == NULL) // null 이 들어올 수 있는 경우 생각해보기!
		ft_strlcpy(target, pwd, PATH_MAX);
	else if (str[0] == '/') // 절대 경로일때
	{
		ft_strlcpy(target, str, PATH_MAX); // 리턴값으로 
		if (ft_strcmp(target, "/") != 0)
		{
			if (target[ft_strlen(target) - 1] == '/')
				target[ft_strlen(target) - 1] = '\0';
		}
	}
	else // 상대 경로일때
	{
		int		i;
		char	**pwd_part = ft_split(pwd, '/');
		char	**str_part = ft_split(str, '/');
		
		// make_part(&pwd_part, pwd, &str_part, str);
		ft_strlcpy(target, pwd, PATH_MAX);
		// pwd 는 항상 업데이트 되므로 중간에 . 이나 .. 이 없다고 가정 -> 중간과정에서도 앞뒤를 확인하는 방식으로!
		i = 0;
		while (str_part[i] != NULL) // ft_split 에서 arr[cnt] = NULL; 로 끝남
		{
			// .here_doc 처럼 .으로만 시작하는 단어때문에 ft_strcmp 써야할듯
			if (str_part[i][0] == '\0' || ft_strcmp(str_part[i], ".") == 0) // . 경로는 무시
			{
				i++;
				continue ;
			}
			else if (ft_strcmp(str_part[i], "..") == 0) // .. 경로는 한칸 위로 올라가야함
			{
				if (ft_strcmp(target, "/") != 0) // 만약 루트라면 올라가지 않음
				{
					// target 끝에는 / 가 붙지않도록 가정
					int	target_len = ft_strlen(target);
					while (target_len > 0 && target[target_len] != '/') // 루트가 아닐때 
					{
						target[target_len] = '\0';
						target_len--;
					}
					if (ft_strcmp(target, "/") != 0) // 지우고 나서 루트가 아닐 경우
						target[target_len] = '\0'; // / 까지 지움
				}
			}
			else // 아닌 경우는 그냥 들어가면 됨
			{
				if (ft_strcmp(target, "/") != 0) // 만약 루트라면 앞에 / 를 붙이지 않음
				{
					target[ft_strlen(target)] = '/'; // 들어가기 전 / 를 앞에 붙여줌
				}
				ft_strlcat(target, str_part[i], PATH_MAX);
			}
			i++;
		}
		free_strs(pwd_part, EXIT_SUCCESS);
		free_strs(str_part, EXIT_SUCCESS);
	}
}

void	update_pwd(char *pwd, char *str)
{
	char buf[PATH_MAX + 1];

	parse_dir(buf, pwd, str);
	ft_memset(pwd, 0, PATH_MAX);
	ft_strlcpy(pwd, buf, PATH_MAX);
}
