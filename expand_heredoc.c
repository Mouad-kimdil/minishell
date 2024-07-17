/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukdid <aboukdid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 01:37:05 by mkimdil           #+#    #+#             */
/*   Updated: 2024/07/16 21:11:47 by aboukdid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_variables(char *str)
{
	char	*exp;
	char	*name;
	char	*end;
	char	*start;
	char	*dollar;
	char	*var_value;

	exp = malloc(sizeof(char) * 1000);
	name = malloc(sizeof(char) * 1000);
	if (!exp || !name)
		return (NULL);
	exp[0] = '\0';
	start = str;
	dollar = ft_strchr(start, '$');
	while (dollar)
	{
		ft_strncat(exp, start, dollar - start);
		if (!ft_isalnum((unsigned char)dollar[1]) && dollar[1] != '_')
		{
			ft_strncat(exp, dollar, 1);
			start = dollar + 1;
			dollar = ft_strchr(start, '$');
			continue ;
		}
		end = dollar + 1;
		while (*end && (ft_isalnum((unsigned char)*end) || *end == '_'))
			end++;
		ft_strncpy(name, dollar + 1, end - dollar - 1);
		name[end - dollar - 1] = '\0';
		var_value = getenv(name);
		if (var_value)
			ft_strcat(exp, var_value);
		start = end;
		dollar = ft_strchr(start, '$');
	}
	ft_strcat(exp, start);
	free(name);
	return (exp);
}

// void	expand_variable_help(t_help *help)
// {
// 	help->dollar = ft_strchr(help->start, '$');
// 	while (help->dollar)
// 	{
// 		ft_strncat(help->exp, help->start, help->dollar - help->start);
// 		if (!ft_isalnum((unsigned char)help->dollar[1])
// 			&& help->dollar[1] != '_')
// 		{
// 			ft_strncat(help->exp, help->dollar, 1);
// 			help->start = help->dollar + 1;
// 			help->dollar = ft_strchr(help->start, '$');
// 			continue ;
// 		}
// 		help->end = help->dollar + 1;
// 		while (*help->end
// 			&& (ft_isalnum((unsigned char)*help->end) || *help->end == '_'))
// 			help->end++;
// 		ft_strncpy(help->name, help->dollar + 1, help->end - help->dollar - 1);
// 		help->name[help->end - help->dollar - 1] = '\0';
// 		help->var_value = getenv(help->name);
// 		if (help->var_value)
// 			ft_strcat(help->exp, help->var_value);
// 		help->start = help->end;
// 		help->dollar = ft_strchr(help->start, '$');
// 	}
// 	ft_strcat(help->exp, help->start);
// }

// char	*expand_variables(char *str)
// {
// 	t_help	*help;

// 	help = malloc(sizeof(t_help));
// 	help->exp = malloc(sizeof(char) * 1000);
// 	help->name = malloc(sizeof(char) * 1000);
// 	if (!help->exp || !help->name)
// 		return (NULL);
// 	help->exp[0] = '\0';
// 	help->start = str;
// 	expand_variable_help(help);
// 	free(help->name);
// 	return (help->exp);
// }
