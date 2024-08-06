/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukdid <aboukdid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 18:16:45 by mkimdil           #+#    #+#             */
/*   Updated: 2024/08/05 12:55:51 by aboukdid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_with_space(t_cmd *lst, char *expanded)
{
	lst->ambiguous = 1;
	lst->argv = join_args(lst->argv, expanded);
}

void	expand_without_space(t_cmd *lst, int *tr, int *i, char *expanded)
{
	(lst->argv[*i]) && (free(lst->argv[*i]), 0);
	lst->argv[*i] = ft_strdup(expanded);
	if (ft_strlen(expanded) == 0 && lst->ambiguous == 0)
	{
		if (*tr != 1 && *tr != 2)
			lst->ambiguous = 1;
		free(lst->argv[*i]);
		lst->argv[*i] = NULL;
	}
}

void	expand_helper(t_cmd *lst, t_list *envp, int *i, int *tr)
{
	char	*ex;

	ex = NULL;
	if (lst->argv[*i] && ft_strsearch(lst->argv[*i], '"'))
		(1) && (*tr = 1, lst->in_quote = 2, 0);
	if (lst->argv[*i] && ft_strsearch(lst->argv[*i], '\''))
		(1) && (lst->in_quote = 2, *tr = 2, 0);
	ex = expand_cmd(lst, envp, *i);
	if (*tr == 1 || *tr == 0)
	{
		if ((ft_strchr(ex, ' ') || ft_strchr(ex, '\t')) && *tr == 0)
			expand_with_space(lst, ex);
		else
			expand_without_space(lst, tr, i, ex);
	}
	if (*tr == 2)
	{
		if (ft_strnstr(lst->argv[*i], "$'"))
			(1) && (free(lst->argv[*i]), lst->argv[*i] = ft_strdup(ex + 1), 0);
		else
			(1) && (free(lst->argv[*i]), lst->argv[*i] = ft_strdup(ex), 0);
	}
	free(ex);
}

void	expand(t_cmd *lst, t_list *envp)
{
	int		i;
	char	*tmp;
	int		tr;

	tr = 0;
	while (lst)
	{
		i = 0;
		tmp = NULL;
		while (lst->argv[i])
		{
			if (ft_strchr(lst->argv[i], '$') && lst->argv[i + 1]
				&& !ft_strchr(lst->argv[i + 1], '$'))
				tmp = lst->argv[i + 1];
			if (ft_strchr(lst->argv[i], '$'))
				expand_helper(lst, envp, &i, &tr);
			if (tmp && !lst->argv[i])
			{
				(lst->argv[i]) && (free(lst->argv[i]), 0);
				lst->argv[i] = ft_strdup(tmp);
			}
			i++;
		}
		lst = lst->next;
	}
}

void	dolar_dolar_case(t_expand *exp, int *j)
{
	char	*temp;

	temp = exp->cmd;
	exp->cmd = ft_strjoin(temp, "\b");
	free(temp);
	(*j) += 2;
}
