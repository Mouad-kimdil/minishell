/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hello.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 03:15:54 by mkimdil           #+#    #+#             */
/*   Updated: 2024/07/24 04:29:22 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef	struct s_hexp
{
	int		tr;
	int		k;
	int		argv_size;
	char	**splited;
	char	*expanded;
	char	*tmp;
}	t_hexp;

void	split_expanded_help(t_cmd *lst, t_hexp *henv, int *i)
{
	lst->argv[*i] = ft_strdup(henv->expanded);
	if (ft_strlen(henv->expanded) == 0 && lst->ambiguous == 0)
	{
		if (henv->tr != 1 && henv->tr != 2)
			lst->ambiguous = 1;
		lst->argv[*i] = NULL;
	}
}

void	split_expanded(t_cmd *lst, t_hexp *henv, int *i, t_list *envp, int *j)
{
	
	henv->expanded = expand_cmd(lst, envp, i);
	if (ft_strsearch(henv->expanded, ' ') && henv->tr == 0)
	{
		lst->ambiguous = 1;
		henv->splited = ft_split(henv->expanded, ' ');
		henv->argv_size = 0;
		while (lst->argv[henv->argv_size])
			henv->argv_size++;
		while (henv->splited[(*j)])
			(*j)++;
		henv->k = henv->argv_size - 1;
		while (--(henv->k) >= (*i))
			lst->argv[henv->k + (*j) - 1] = lst->argv[henv->k];
		*j = 0;
		henv->k = (*i);
		while (henv->splited[*j])
			lst->argv[(henv->k)++] = ft_strdup(henv->splited[(*j)++]);
	}
	else
		split_expanded_help(lst, henv, &i);
}

void	expand_help(t_cmd *lst, t_hexp *henv, t_list *envp, int *i, int *j)
{
	if (ft_strsearch(lst->argv[*i], '"'))
		henv->tr = 1;
	if (ft_strsearch(lst->argv[*i], '\''))
		henv->tr = 2;
	if (henv->tr == 1 || henv->tr == 0)
		split_expanded(lst, henv, &i, envp, &j);
}

void	expand(t_cmd *lst, t_list *envp)
{
	t_hexp	henv;
	int		i;
	int		j;

	henv.tr = 0;
	j = 0;
	while (lst)
	{
		i = -1;
		while (lst->argv[++i])
		{
			if (ft_strchr(lst->argv[i], '$') && lst->argv[i + 1]
				&& !ft_strchr(lst->argv[i + 1], '$'))
				henv.tmp = lst->argv[i + 1];
			if (ft_strchr(lst->argv[i], '$'))
				expand_help(lst, &henv, envp, &i, &j);
			if (henv.tmp && !lst->argv[i])
				lst->argv[i] = ft_strdup(henv.tmp);
		}
		lst = lst->next;
	}
}
