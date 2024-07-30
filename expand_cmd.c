/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 04:57:34 by mkimdil           #+#    #+#             */
/*   Updated: 2024/07/30 02:56:21 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_single_quote(t_expand *exp, int *j)
{
	(*j)++;
	while (exp->current[*j] && exp->current[*j] != '\'')
	{
		exp->cmd = ft_strjoin(exp->cmd, ft_substr(exp->current, *j, 1));
		(*j)++;
	}
	if (exp->current[*j] == '\'')
		(*j)++;
}

void	handle_double_quote(t_expand *exp, int *j, int *k, t_list *envp)
{
	(*j)++;
	while (exp->current[*j] && exp->current[*j] != '"')
	{
		if (exp->current[*j] == '$' && special_case(exp->current[(*j) + 1]))
		{
			(*j)++;
			*k = *j;
			while (exp->current[*j] && special_case(exp->current[*j]))
				(*j)++;
			exp->name = ft_substr(exp->current, *k, *j - *k);
			exp->value = get_env_value(exp->name, envp->envs);
			exp->cmd = ft_strjoin(exp->cmd, exp->value);
			free(exp->name);
		}
		else
		{
			exp->cmd = ft_strjoin(exp->cmd, ft_substr(exp->current, *j, 1));
			(*j)++;
		}
	}
	if (exp->current[*j] == '"')
		(*j)++;
}

void	handle_special_case(t_expand *exp, int *j, int *k, t_list *envp)
{
	(*j)++;
	*k = *j;
	while (exp->current[*j] && special_case(exp->current[*j]))
		(*j)++;
	exp->name = ft_substr(exp->current, *k, *j - *k);
	exp->value = get_env_value(exp->name, envp->envs);
	exp->cmd = ft_strjoin(exp->cmd, exp->value);
	free(exp->name);
}

char	*expand_cmd(t_cmd *lst, t_list *envp, int i)
{
	t_expand	exp;
	int			j;
	int			k;

	exp.cmd = ft_strdup("");
	exp.current = lst->argv[i];
	j = 0;
	k = 0;
	while (exp.current[j])
	{
		if (exp.current[j] == '$' && exp.current[j + 1] == '?')
			j++;
		else if (exp.current[j] == '\'')
			handle_single_quote(&exp, &j);
		else if (exp.current[j] == '"')
			handle_double_quote(&exp, &j, &k, envp);
		else if (exp.current[j] == '$' && special_case(exp.current[j + 1]))
			handle_special_case(&exp, &j, &k, envp);
		else if (exp.current[j] == '$' && exp.current[j + 1] == '$')
			j += 2;
		else if (exp.current[j] == '$' && exp.current[j + 1] == '"')
			j++;
		else
			exp.cmd = ft_strjoin(exp.cmd, ft_substr(exp.current, j, 1)), j++;
	}
	return (exp.cmd);
}
