/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 01:37:05 by mkimdil           #+#    #+#             */
/*   Updated: 2024/07/29 23:53:21 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(char *name, t_env *env)
{
	while (env)
	{
		if (ft_strcmp(name, env->name) == 0)
			return (env->value);
		env = env->next;
	}
	return ("");
}

char	*handle_dollar_sign(char *curr, char *cmd, int *j, t_list *envp)
{
	int		k;
	char	*name;
	char	*value;

	if (curr[*j + 1] == '?' || curr[*j + 1] == '$' || curr[*j + 1] == '"')
		(*j)++;
	else if (special_case(curr[*j + 1]))
	{
		(*j)++;
		k = *j;
		while (curr[*j] && special_case(curr[*j]))
			(*j)++;
		name = ft_substr(curr, k, *j - k);
		value = get_env_value(name, envp->envs);
		cmd = ft_strjoin(cmd, value);
		free(name);
	}
	return (cmd);
}

char	*handle_other_cases(char *curr, char *cmd, int *j)
{
	cmd = ft_strjoin(cmd, ft_substr(curr, *j, 1));
	(*j)++;
	return (cmd);
}

char	*expand_here_cmd(char *temp, t_list *envp)
{
	char	*cmd;
	char	*curr;
	int		j;

	cmd = ft_strdup("");
	curr = temp;
	j = 0;
	while (curr[j])
	{
		if (curr[j] == '$')
			cmd = handle_dollar_sign(curr, cmd, &j, envp);
		else
			cmd = handle_other_cases(curr, cmd, &j);
	}
	return (cmd);
}

char	*expand_heredoc(char *temp, t_list *envp)
{
	char	*expanded;

	expanded = NULL;
	if (ft_strchr(temp, '$'))
		expanded = expand_here_cmd(temp, envp);
	if (!expanded)
		expanded = ft_strdup(temp);
	return (expanded);
}
