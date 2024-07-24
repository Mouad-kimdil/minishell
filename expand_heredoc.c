/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 01:37:05 by mkimdil           #+#    #+#             */
/*   Updated: 2024/07/24 03:51:17 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_dollar_sign(char *current, char *cmd, int *j, t_list *envp)
{
	int		k;
	char	*var_name;
	char	*value;

	if (current[*j + 1] == '?' || current[*j + 1] == '$' || current[*j + 1] == '"')
		(*j)++;
	else if (special_case(current[*j + 1]))
	{
		(*j)++;
		k = *j;
		while (current[*j] && special_case(current[*j]))
			(*j)++;
		var_name = ft_substr(current, k, *j - k);
		value = get_env_value(var_name, envp->envs);
		cmd = ft_strjoin(cmd, value);
		free(var_name);
	}
	return (cmd);
}

char	*handle_other_cases(char *current, char *cmd, int *j)
{
	cmd = ft_strjoin(cmd, ft_substr(current, *j, 1));
	(*j)++;
	return cmd;
}

char	*expand_here_cmd(char *temp, t_list *envp)
{
	char	*cmd;
	char	*current;
	int		j;

	cmd = ft_strdup("");
	current = temp;
	j = 0;
	while (current[j])
	{
		if (current[j] == '$')
			cmd = handle_dollar_sign(current, cmd, &j, envp);
		else
			cmd = handle_other_cases(current, cmd, &j);
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
		expanded = temp;
	return (expanded);
}
