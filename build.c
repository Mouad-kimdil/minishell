/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 12:07:53 by aboukdid          #+#    #+#             */
/*   Updated: 2024/07/17 05:21:44 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	build_arr_help(t_cmd **lst, char *res)
{
	(*lst)->cmd = nops_strdup(res);
	(*lst)->argv = ft_split((*lst)->cmd, ' ');
	(*lst)->infile = 0;
	(*lst)->outfile = 1;
	(*lst)->next = NULL;
}

t_cmd	*build_arr(char **res)
{
	int		i;
	t_cmd	*node;
	t_cmd	*final;
	t_cmd	*new_node;

	i = -1;
	final = NULL;
	while (res[++i])
	{
		new_node = malloc(sizeof(t_cmd));
		if (!new_node)
			exit(EXIT_FAILURE);
		build_arr_help(&new_node, res[i]);
		if (final == NULL)
		{
			final = new_node;
			node = final;
		}
		else
		{
			node->next = new_node;
			node = node->next;
		}
	}
	return (final);
}

void	remove_quotes_from_arg(char *arg)
{
	int		len;
	int		i;
	int		j;
	int		tr;
	char	qoutes;

	len = ft_strlen(arg);
	i = 0;
	j = 0;
	tr = 0;
	if (arg[0] == '\'' || arg[0] == '\"')
		(1) && (qoutes = arg[0], tr = 1, 0);
	while (i < len)
	{
		if (arg[i] == qoutes && tr == 1)
		{
			i++;
			continue ;
		}
		if (arg[i] != qoutes)
			arg[j++] = arg[i];
		i++;
	}
	arg[j] = '\0';
}

void	process_argv(char **argv)
{
	while (*argv != NULL)
	{
		remove_quotes_from_arg(*argv);
		argv++;
	}
}

void	remove_qoutes(t_cmd **lst)
{
	t_cmd	*current;

	current = *lst;
	while (current != NULL)
	{
		process_argv(current->argv);
		current = current->next;
	}
}
