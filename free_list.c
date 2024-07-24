/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 04:16:45 by mkimdil           #+#    #+#             */
/*   Updated: 2024/07/21 23:59:14 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		free_arr((*lst)->delim);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}
