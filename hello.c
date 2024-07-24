/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hello.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 03:15:54 by mkimdil           #+#    #+#             */
/*   Updated: 2024/07/24 03:29:27 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_expand
{
	char	*cmd;
	char	*current;
	char	*var_name;
	char	*value;
}	t_expand;

void	fun1(t_expand *exp, t_cmd *lst, int *j)
{
	(*j)++;
	while (exp->current[*j] && exp->current[*j] != '\'')
		exp->cmd = ft_strjoin(exp->cmd, ft_substr(exp->current, j, 1)), (*j)++;
	if (exp->current[*j] == '\'')
		(*j)++;
}

void	fun2(t_expand *exp, t_cmd *lst, int *j, int *k, t_list *envp)
{
	(*j)++;
	while (exp->current[*j] && exp->current[*j] != '"')
	{
		if (exp->current[*j] == '$' && special_case(exp->current[(*j) + 1]))
		{
			(*j)++, *k = *j;
			while (exp->current[*j] && special_case(exp->current[*j]))
				(*j)++;
			exp->var_name = ft_substr(exp->current, (*k), (*j) - (*k));
			exp->value = get_env_value(exp->var_name, envp->envs);
			exp->cmd = ft_strjoin(exp->cmd, exp->value), free(exp->var_name);
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

void	fun3(t_expand *exp, t_cmd *lst, int *j, int *k, t_list *envp)
{
	(*j)++, *k = *j;
	while (exp->current[*j] && special_case(exp->current[*j]))
		(*j)++;
	exp->var_name = ft_substr(exp->current, *k, (*j) - (*k));
	exp->value = get_env_value(exp->var_name, envp->envs);
	exp->cmd = ft_strjoin(exp->cmd, exp->value);
	free(exp->var_name);
}

char	*expand_cmd(t_cmd *lst, t_list *envp, int i)
{
	t_expand	*exp;
	int			i;
	int			j;
	int			k;

	exp->cmd = ft_strdup(""), exp->current = lst->argv[i], j = 0, k = 0;
	while (exp->current[j])
	{
		if (exp->current[j] == '$' && exp->current[j + 1] == '?')
			j++;
		else if (exp->current[j] && exp->current[j] != '\'')
			fun1(exp, lst, &j);
		else if (exp->current[j] == '"')
			fun2(exp, lst, &j, &k, envp);
		else if (exp->current[j] == '$' && special_case(exp->current[j + 1]))
			fun3(exp, lst, &j, &k, envp);
		else if (exp->current[j] == '$' && exp->current[j + 1] == '$')
			j += 2;
		else if (exp->current[j] == '$' && exp->current[j + 1] == '"')
			j++;
		else
			exp->cmd = ft_strjoin(exp->cmd, ft_substr(exp->current, j, 1)), j++;
	}
	return (exp->cmd);
}

int	main(void)
{

}
