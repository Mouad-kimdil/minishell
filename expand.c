/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 18:16:45 by mkimdil           #+#    #+#             */
/*   Updated: 2024/07/24 03:46:09 by mkimdil          ###   ########.fr       */
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

char	*expand_cmd(t_cmd *lst, t_list *envp, int i)
{
	char	*cmd;
	char	*current;
	char	*var_name;
	char	*value;
	int		j;
	int		k;
	
	cmd = ft_strdup("");
	current = lst->argv[i];
	j = 0;
	while (current[j])
	{
		if (current[j] == '$' && current[j + 1] == '?')
			j++;
		else if (current[j] == '\'')
		{
			j++;
			while (current[j] && current[j] != '\'')
			{
				cmd = ft_strjoin(cmd, ft_substr(current, j, 1));
				j++;
			}
			if (current[j] == '\'')
				j++;
		}
		else if (current[j] == '"')
		{
			j++;
			while (current[j] && current[j] != '"')
			{
				if (current[j] == '$' && special_case(current[j + 1]))
				{
					j++;
					k = j;
					while (current[j] && special_case(current[j]))
						j++;
					var_name = ft_substr(current, k, j - k);
					value = get_env_value(var_name, envp->envs);
					cmd = ft_strjoin(cmd, value);
					free(var_name);
				}
				else
				{
					cmd = ft_strjoin(cmd, ft_substr(current, j, 1));
					j++;
				}
			}
			if (current[j] == '"')
				j++;
		}
		else if (current[j] == '$' && special_case(current[j + 1]))
		{
			j++;
			k = j;
			while (current[j] && special_case(current[j]))
				j++;
			var_name = ft_substr(current, k, j - k);
			value = get_env_value(var_name, envp->envs);
			cmd = ft_strjoin(cmd, value);
			free(var_name);
		}
		else if (current[j] == '$' && current[j + 1] == '$')
			j += 2;
		else if (current[j] == '$' && current[j + 1] == '"')
			j++;
		else
		{
			cmd = ft_strjoin(cmd, ft_substr(current, j, 1));
			j++;
		}
	}
	return (cmd);
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
