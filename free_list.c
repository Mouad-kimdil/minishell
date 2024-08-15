#include "minishell.h"

void	free_list(t_env **env)
{
	t_env	*tmp;

	if (!env || !(*env))
		return ;
	while (*env)
	{
		tmp = (*env)->next;
		free((*env)->name);
		free((*env)->value);
		free(*env);
		*env = tmp;
	}
	*env = NULL;
}

void	free_list1(t_cmd **lst)
{
	t_cmd	*tmp;

	if (!lst || !(*lst))
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		free_arr((*lst)->argv);
		free((*lst)->cmd);
		free_arr((*lst)->del);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}
