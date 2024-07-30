#include "../parser.h"

int sort_strcmp(t_envp_list *pn, t_envp_list *n)
{
    const int  s1_len = ft_strlen(pn->key);
    const int  s2_len = ft_strlen(n->key);
    if (s1_len > s2_len)
        return (ft_strncmp(pn->key, n->key, s1_len + 1));
    else
        return (ft_strncmp(pn->key, n->key, s2_len + 1));
}


void    node_swap(t_envp_list *a, t_envp_list *b) {
    char *temp_key = a->key;
    char *temp_value = a->value;

    a->key = b->key;
    a->value = b->value;
    b->key = temp_key;
    b->value = temp_value;
}

void    bubble_sort(t_envp_list *head)
{
    int is_swap;
    t_envp_list *cur;
    t_envp_list *last = NULL;

    if (head == NULL)
        return;

    while (1)
    {
		is_swap = 0;
		cur = head;
		while (cur->next != last)
		{
			if (sort_strcmp(cur, cur->next) > 0)
			{
				node_swap(cur, cur->next);
				is_swap = 1;
			}
		    cur = cur->next;
		}
		last = cur;
		if (is_swap == 0)
			break ;
	}
}
