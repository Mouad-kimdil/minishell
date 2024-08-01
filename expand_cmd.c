/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 04:57:34 by mkimdil           #+#    #+#             */
/*   Updated: 2024/08/01 04:16:10 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_single_quote(t_expand *exp, int *j)
{
	char	*temp;
	char	*temp1;

	temp = NULL;
	(*j)++;
	while (exp->current[*j] && exp->current[*j] != '\'')
	{
		temp = ft_substr(exp->current, *j, 1);
		temp1 = exp->cmd;
		exp->cmd = ft_strjoin(temp1, temp);
		free(temp);
		free(temp1);
		(*j)++;
	}
	if (exp->current[*j] == '\'')
		(*j)++;
}

void	handle_double_quote(t_expand *exp, int *j, int *k, t_list *envp)
{
	char	*temp1;
	char	*temp;

	(1) && (temp1 = NULL, temp = NULL, (*j)++, 0);
	while (exp->current[*j] && exp->current[*j] != '"')
	{
		if (exp->current[*j] == '$' && special_case(exp->current[(*j) + 1]))
		{
			(1) && ((*j)++, *k = *j, 0);
			while (exp->current[*j] && special_case(exp->current[*j]))
				(*j)++;
			exp->name = ft_substr(exp->current, *k, *j - *k);
			exp->value = get_env_value(exp->name, envp->envs);
			(1) && (temp = exp->cmd, free(exp->name), 0);
			(1) && (exp->cmd = ft_strjoin(temp, exp->value), free(temp), 0);
		}
		else
		{
			(1) && (temp1 = ft_substr(exp->current, *j, 1), free(exp->cmd), 0);
			(1) && (exp->cmd = NULL, exp->cmd = ft_strjoin(exp->cmd, temp1), 0);
			(1) && (free(temp1), (*j)++, 0);
		}
	}
	if (exp->current[*j] == '"')
		(*j)++;
}

void	handle_special_case(t_expand *exp, int *j, int *k, t_list *envp)
{
	char	*temp;

	temp = NULL;
	(*j)++;
	*k = *j;
	while (exp->current[*j] && special_case(exp->current[*j]))
		(*j)++;
	exp->name = ft_substr(exp->current, *k, *j - *k);
	exp->value = get_env_value(exp->name, envp->envs);
	free(exp->name);
	temp = exp->cmd;
	exp->cmd = ft_strjoin(temp, exp->value);
	free(temp);
}

void	last_case(t_expand *exp, int *j)
{
	char	*temp1;

	temp1 = exp->cmd;
	exp->temp = ft_substr(exp->current, *j, 1);
	exp->cmd = ft_strjoin(temp1, exp->temp);
	free(exp->temp);
	(*j)++;
}

char	*expand_cmd(t_cmd *lst, t_list *envp, int i)
{
	t_expand	exp;
	int			j;
	int			k;

	(1) && (exp.cmd = NULL, exp.current = lst->argv[i], j = 0, k = 0, 0);
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
			last_case(&exp, &j);
	}
	return (exp.cmd);
}
