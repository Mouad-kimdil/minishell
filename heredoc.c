/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukdid <aboukdid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 14:55:09 by mkimdil           #+#    #+#             */
/*   Updated: 2024/07/29 15:02:33 by aboukdid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_delim(t_cmd *lst)
{
	int		i;
	int		k;
	int		delim_size;
	char	*tmp1;

	delim_size = get_delim_size(lst) * 2;
	lst->delim = malloc(sizeof(char *) * delim_size + 1);
	if (!lst->delim)
		return ;
	i = -1;
	k = 0;
	while (lst->argv[++i] && lst->argv[i + 1])
	{
		if (!ft_strcmp(lst->argv[i], "<<"))
		{
			if (ft_strchr(lst->argv[i + 1], '\'')
				|| ft_strchr(lst->argv[i + 1], '\"'))
				lst->in_quote = 1;
			tmp1 = unquote(lst->argv[i + 1]);
			lst->delim[k] = ft_strdup(tmp1);
			free(tmp1);
			k++;
		}
	}
	lst->delim[k] = NULL;
}

char	*creat_heroc(t_cmd *lst)
{
	int		i;
	char	*tmp;
	char	*tmp1;

	i = 0;
	while (++i)
	{
		tmp1 = ft_itoa(i);
		tmp = ft_strjoin("heredoc", tmp1);
		if (!tmp || !tmp1)
			return (free(tmp), free(tmp1), NULL);
		free(tmp1);
		lst->fd = open(tmp, O_RDONLY, 0644);
		if (lst->fd == -1)
		{
			lst->fd = open(tmp, O_RDWR | O_CREAT | O_TRUNC, 0644);
			if (lst->fd < 0)
				return (free(tmp), free(tmp1), NULL);
			return (tmp);
		}
	}
	return (NULL);
}

void	perferm_heredoc_help(int fd, char *exp, int in)
{
	if (exp && in != 1)
	{
		write(fd, exp, ft_strlen(exp));
		write(fd, "\n", 1);
		free(exp);
	}
}

int	perferm_heredoc(t_cmd *lst, int in, char *delim, t_list *env)
{
	char	*exp;
	char	*tmp;

	while (1)
	{
		signal(SIGINT, her_sin);
		tmp = readline("> ");
		if (!ttyname(0))
		{
			open(ttyname(2), O_RDWR);
			return (free(tmp), 1);
		}
		if (!tmp || ((ft_strncmp(tmp, delim, ft_strlen(delim)) == 0)
				&& (ft_strlen(tmp) == ft_strlen(delim))))
			{
				free(tmp);
				break ;
			}
		exp = expand_heredoc(tmp, env);
		free(tmp);
		perferm_heredoc_help(lst->fd, exp, in);
	}
	return (0);
}

void	heredoc(t_cmd *lst, t_list *env)
{
	char	*tmp;
	int		i;

	while (lst)
	{
		i = -1;
		get_delim(lst);
		while (lst->delim[++i])
		{
			tmp = creat_heroc(lst);
			if (perferm_heredoc(lst, lst->in_quote, lst->delim[i], env))
			{
				free(lst->delim[i]), unlink(tmp), free(tmp);
				close(lst->fd);
				break ;
			}
			if (lst->infile != 0)
				close(lst->infile);
			lst->infile = open(tmp, O_RDONLY);
			free(lst->delim[i]), close(lst->fd), unlink(tmp), free(tmp);
		}
		free(lst->delim);
		lst = lst->next;
	}
}
