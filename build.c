/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 12:07:53 by aboukdid          #+#    #+#             */
/*   Updated: 2024/07/21 01:01:38 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void build_arr_help(t_cmd **lst, char *res)
{
    char **splited;
    int i;

    splited = ft_split(res, ' ');
    (*lst)->argv = malloc(1000 * sizeof(char *));
    if (!(*lst)->argv)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    i = 0;
    while (splited[i])
    {
        (*lst)->argv[i] = ft_strdup(splited[i]);
        i++;
    }
    (*lst)->argv[i] = NULL;
    (*lst)->cmd = ft_strdup(res);
    (*lst)->infile = 0;
    (*lst)->outfile = 1;
    (*lst)->ambiguous = 0;
    (*lst)->next = NULL;
    free(splited);
}

t_cmd *build_arr(char **res)
{
    int i;
    t_cmd *node;
    t_cmd *final;
    t_cmd *new_node;

    i = -1;
    final = NULL;
    while (res[++i])
    {
        new_node = malloc(sizeof(t_cmd));
        if (!new_node)
        {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
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
	int		i;
	int		j;
	int		tr;
	char	qoutes;

    if (count_single(arg) == 1 && count_double(arg) == 0)
		return  ;
	i = -1;
	j = 0;
	tr = 0;
	while (arg[++i])
	{
		if (arg[i] == '\'' || arg[i] == '\"')
		{
			qoutes = arg[i], tr = 1;
			break ;
		}
	}
	i = -1;
	while (++i < ft_strlen(arg))
	{
		if (arg[i] == qoutes && tr == 1)
		{
			tr = 0;
			continue ;
		}
		if (arg[i] != qoutes)
			arg[j++] = arg[i];
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
