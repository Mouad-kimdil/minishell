/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukdid <aboukdid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 12:07:53 by aboukdid          #+#    #+#             */
/*   Updated: 2024/07/29 19:57:59 by aboukdid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*ft_new(char *cmd)
{
	t_cmd	*new;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	if (new)
	{
		new->cmd = ft_strdup(cmd);
		if (!new->cmd)
			return (NULL);
		new->argv = ft_split_2(new->cmd);
		if (!new->argv)
			return (NULL);
		new->infile = 0;
		new->outfile = 1;
		new->ambiguous = 0;
		new->in_quote = -1;
		new->is_heredoc = -1;
		new->next = NULL;
	}
	return (new);
}

t_cmd	*get_last(t_cmd *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

void	add_back(t_cmd **lst, t_cmd *new)
{
	t_cmd	*last;

	if (*lst == NULL)
		*lst = new;
	else
	{
		last = get_last(*lst);
		last->next = new;
	}
}

t_cmd	*build_arr(char **res)
{
	t_cmd	*head;
	int		i;

	head = NULL;
	i = 0;
	while (res[i])
	{
		add_back(&head, ft_new(res[i]));
		i++;
	}
	return (head);
}
