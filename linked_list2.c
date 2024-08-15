#include "minishell.h"

t_exp	*ft_new_node(char *str)
{
	t_exp	*new;

	new = malloc(sizeof(t_exp));
	if (new)
	{
		new->splited = ft_split_2(str);
		new->next = NULL;
	}
	return (new);
}

t_exp	*last_node(t_exp *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

void	ft_add_back(t_exp **lst, t_exp *new)
{
	t_exp	*last;

	if (*lst == NULL)
		*lst = new;
	else
	{
		last = last_node(*lst);
		last->next = new;
	}
}
