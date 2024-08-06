/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukdid <aboukdid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 16:50:50 by mkimdil           #+#    #+#             */
/*   Updated: 2024/08/03 13:22:10 by aboukdid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*duplicate(char *str)
{
	char	*ptr;
	int		i;
	int		j;

	ptr = malloc(1 + ft_strlen(str));
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			i++;
			continue ;
		}
		ptr[j] = str[i];
		i++;
		j++;
	}
	ptr[j] = '\0';
	return (ptr);
}

void	free_exp(t_expand *exp)
{
	t_expand	*curr;

	curr = exp;
	free(curr->current);
	free(curr->name);
	free(curr->value);
	free(curr);
}

void	free_list(t_list *list)
{
	t_env	*current;
	t_env	*next;

	if (!list)
		return ;
	current = list->envs;
	while (current)
	{
		next = current->next;
		free(current->name);
		free(current->value);
		free(current);
		current = next;
	}
	free(list);
}

void	f_cmd(t_cmd **lst)
{
	t_cmd	*next;

	if (!lst || !*lst)
		return ;
	while (*lst)
	{
		next = (*lst)->next;
		free((*lst)->cmd);
		fr((*lst)->argv);
		free(*lst);
		*lst = next;
	}
	*lst = NULL;
}

int	ft_strsearch(char *s, int c)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (i < ft_strlen(s) + 1)
	{
		if (s[i] == (char)c)
			return (1);
		i++;
	}
	return (0);
}
