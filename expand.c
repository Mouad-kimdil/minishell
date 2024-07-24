/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 18:16:45 by mkimdil           #+#    #+#             */
/*   Updated: 2024/07/24 03:43:23 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	special_case(char c)
{
	return (is_ascii(c) || is_number(c) || c == '_');
}

char	*get_env_value(char *var_name, t_env *env)
{
	while (env)
	{
		if (ft_strcmp(var_name, env->name) == 0)
			return (env->value);
		env = env->next;
	}
	return ("");
}

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
			exp->var_name = ft_substr(exp->current, *k, *j - *k);
			exp->value = get_env_value(exp->var_name, envp->envs);
			exp->cmd = ft_strjoin(exp->cmd, exp->value);
			free(exp->var_name);
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
	exp->var_name = ft_substr(exp->current, *k, *j - *k);
	exp->value = get_env_value(exp->var_name, envp->envs);
	exp->cmd = ft_strjoin(exp->cmd, exp->value);
	free(exp->var_name);
}

char	*expand_cmd(t_cmd *lst, t_list *envp, int i)
{
	t_expand	exp;
	int		j;
	int		k;

	exp.cmd = ft_strdup("");
	exp.current = lst->argv[i];
	j = 0, k = 0;
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

void	expand(t_cmd *lst, t_list *envp)
{
	int		i;
	int		j;
	int		k;
	int		tr;
    int		argv_size;
    char	*expanded;
    char	**splited;
	char	*tmp;

	tr = 0;
	tmp = NULL;
	while (lst)
	{
		i = 0;
		while (lst->argv[i])
		{
			if (ft_strchr(lst->argv[i], '$') && lst->argv[i + 1]
				&& !ft_strchr(lst->argv[i + 1], '$'))
				tmp = lst->argv[i + 1];
			if (ft_strchr(lst->argv[i], '$'))
			{
				if (ft_strsearch(lst->argv[i], '"'))
					tr = 1;
				if (ft_strsearch(lst->argv[i], '\''))
					tr = 2;
				if (tr == 1 || tr == 0)
				{
					expanded = expand_cmd(lst, envp, i);
					if (ft_strsearch(expanded, ' ') && tr == 0)
					{
						lst->ambiguous = 1;
						splited = ft_split(expanded, ' ');
						argv_size = 0;
						while (lst->argv[argv_size])
							argv_size++;
						j = 0;
						while (splited[j])
							j++;
						k = argv_size;
						while (k >= i)
						{
							lst->argv[k + j - 1] = lst->argv[k];
							k--;
						}
						j = 0;
						k = i;
						while (splited[j])
						{
							lst->argv[k] = ft_strdup(splited[j]);
							k++;
							j++;
						}
					}
					else
					{
						lst->argv[i] = ft_strdup(expanded);
						if (ft_strlen(expanded) == 0 && lst->ambiguous == 0)
						{
							if (tr != 1 && tr != 2)
								lst->ambiguous = 1;
							lst->argv[i] = NULL;
						}
					}
				}
			}
			if (tmp && !lst->argv[i])
				lst->argv[i] = ft_strdup(tmp);
			i++;
		}
		lst = lst->next;
	}
}
