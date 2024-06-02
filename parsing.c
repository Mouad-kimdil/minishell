/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 13:58:20 by mkimdil           #+#    #+#             */
/*   Updated: 2024/06/01 13:54:05 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	syn_error3(char **res)
{
	if (!ft_strncmp(res[0], ">", 2) && !ft_strncmp(res[1], "|", 1))
		return (printf("bash: syntax error near unexpected token `|'\n"), 1);
	else if (!ft_strncmp(res[0], ">", 2) && !ft_strncmp(res[1], "<<", 2))
		return (printf("bash: syntax error near unexpected token `<<'\n"), 1);
	else if (!ft_strncmp(res[0], ">", 2) && !ft_strncmp(res[1], "<", 1))
		return (printf("bash: syntax error near unexpected token `<'\n"), 1);
	else if (!ft_strncmp(res[0], ">", 2) && !ft_strncmp(res[1], ">>", 2))
		return (printf("bash: syntax error near unexpected token `>>'\n"), 1);
	else if (!ft_strncmp(res[0], ">", 2) && !ft_strncmp(res[1], ">", 1))
		return (printf("bash: syntax error near unexpected token `>'\n"), 1);
	else if (ft_strncmp(*res, "<<", 3) == 0 && ft_strncmp(res[1], ">", 1) == 0)
		return (printf("bash: syntax error near unexpected token `>'\n"), 1);
	else if (ft_strncmp(res[0], "(", 2) == 0)
		return (printf("bash: syntax error\n"), 1);
	else if (ft_strncmp(res[0], ")", 2) == 0)
		return (printf("bash: syntax error\n"), 1);
	free_array(res);
	return (0);
}

int	syn_error2(char **res)
{
	if (**res == '|')
		return (printf("bash: syntax error near unexpected token `|'\n"), 1);
	if (!ft_strncmp(res[0], "<", 2) && !ft_strncmp(res[1], "|", 1))
		return (printf("bash: syntax error near unexpected token `|'\n"), 1);
	else if (!ft_strncmp(res[0], "<", 2) && !ft_strncmp(res[1], "<<", 2))
		return (printf("bash: syntax error near unexpected token `<<'\n"), 1);
	else if (!ft_strncmp(res[0], "<", 2) && !ft_strncmp(res[1], "<", 1))
		return (printf("bash: syntax error near unexpected token `<'\n"), 1);
	else if (!ft_strncmp(res[0], "<", 2) && !ft_strncmp(res[1], ">>", 2))
		return (printf("bash: syntax error near unexpected token `>>'\n"), 1);
	else if (!ft_strncmp(res[0], "<", 2) && !ft_strncmp(res[1], ">", 1))
		return (printf("bash: syntax error near unexpected token `>'\n"), 1);
	else if (!ft_strncmp(res[0], ">>", 3) && !ft_strncmp(res[1], "|", 1))
		return (printf("bash: syntax error near unexpected token `|'\n"), 1);
	else if (!ft_strncmp(res[0], ">>", 3) && !ft_strncmp(res[1], ">", 1))
		return (printf("bash: syntax error near unexpected token `>'\n"), 1);
	else if (!ft_strncmp(res[0], ">>", 3) && !ft_strncmp(res[1], ">>", 2))
		return (printf("bash: syntax error near unexpected token `>>'\n"), 1);
	else if (!ft_strncmp(res[0], ">>", 3) && !ft_strncmp(res[1], "<", 1))
		return (printf("bash: syntax error near unexpected token `<'\n"), 1);
	else if (!ft_strncmp(res[0], ">>", 3) && !ft_strncmp(res[1], "<<", 2))
		return (printf("bash: syntax error near unexpected token `<<'\n"), 1);
	if (syn_error3(res))
		return (1);
	return (0);
}

int	check_end(char *line)
{
	if (line[ft_strlen(line) - 1] == '|' || line[ft_strlen(line) - 1] == '>' 
		|| (line[ft_strlen(line) - 1] == '<')
		|| line[ft_strlen(line) - 1] == ')'
		|| line[ft_strlen(line) - 1] == '(')
		return (1);
	return (0);
}

int	syn_error(char *line)
{
	char	**res;

	res = ft_split(line, ' ');
	if (!res || !*res)
		return (1);
	if ((!ft_strncmp(res[0], "<<", 3) || !ft_strncmp(res[0], ">>", 3)
			|| !ft_strncmp(res[0], "<", 2) || !ft_strncmp(res[0], ">", 2))
		&& !*(res + 1))
		return (printf("bash: syntax error near \
		 unexpected token `newline'\n"), 1);
	else if (ft_strncmp(*res, "<<", 3) == 0
		&& (!*(res + 1) || res[1][0] == '|'))
		return (printf("bash: syntax error near unexpected token `|'\n"), 1);
	else if (ft_strncmp(*res, "<<", 3) == 0 && ft_strncmp(res[1], "<<", 2) == 0)
		return (printf("bash: syntax error near unexpected token `<<'\n"), 1);
	else if (ft_strncmp(*res, "<<", 3) == 0 && ft_strncmp(res[1], "<", 1) == 0)
		return (printf("bash: syntax error near unexpected token `<'\n"), 1);
	else if (ft_strncmp(*res, "<<", 3) == 0 && ft_strncmp(res[1], ">>", 2) == 0)
		return (printf("bash: syntax error near unexpected token `>>'\n"), 1);
	else if (check_end(line))
		return (printf("bash: syntax error\n"), 1);
	if (syn_error2(res))
		return (1);
	return (0);
}
