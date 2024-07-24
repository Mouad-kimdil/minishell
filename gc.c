/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukdid <aboukdid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 00:24:37 by aboukdid          #+#    #+#             */
/*   Updated: 2024/07/16 13:13:52 by aboukdid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clear_gc(t_gc **gc)
{
	t_gc	*tmp;

	while (*gc)
	{
		tmp = *gc;
		*gc = (*gc)->next;
		free(tmp->ptr);
		free(tmp);
	}
}

void	*gc(int size, int mode)
{
	static t_gc	*gc;
	t_gc		*new;

	if (mode == 1)
	{
		new = malloc(sizeof(t_gc));
		if (!new)
			return (NULL);
		new->ptr = malloc(size);
		if (!new->ptr)
			return (free(new), NULL);
		new->next = gc;
		gc = new;
		return (new->ptr);
	}
	else if (mode == 0)
		clear_gc(&gc);
	return (NULL);
}
