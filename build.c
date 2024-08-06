/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 12:07:53 by aboukdid          #+#    #+#             */
/*   Updated: 2024/08/03 00:26:09 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_parse(t_parse *p)
{
	if (!p)
		return ;
	fr(p->res);
	free(p->str);
	free(p->temp);
}

t_cmd	*ft_new(char *cmd)
{
	t_cmd	*new;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->cmd = ft_strdup(cmd);
	if (!new->cmd)
		return (free(new), NULL);
	new->argv = ft_split_2(new->cmd);
	if (!new->argv)
		return (free(new->cmd), free(new), NULL);
	new->inf = 0;
	new->outfile = 1;
	new->ambiguous = 0;
	new->in_quote = -1;
	new->is_heredoc = -1;
	new->next = NULL;
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
	t_cmd	*node;
	int		i;

	head = NULL;
	node = NULL;
	i = 0;
	while (res[i])
	{
		node = ft_new(res[i]);
		if (!node)
			return (fr(res), NULL);
		add_back(&head, node);
		i++;
	}
	return (head);
}
