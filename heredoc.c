/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 14:55:09 by mkimdil           #+#    #+#             */
/*   Updated: 2024/07/22 22:39:01 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_delim(t_cmd *lst)
{
	int	i;
	int	k;
	int	delim_size;

	delim_size = get_delim_size(lst) * 2;
	lst->delim = malloc(sizeof(char *) * delim_size + 1);
	if (!lst->delim)
		return ;
	i = 0;
	k = 0;
	while (lst->argv[i] && lst->argv[i + 1])
	{
		if (!ft_strcmp(lst->argv[i], "<<"))
		{
			if (ft_strchr(lst->argv[i + 1], '\'')
				|| ft_strchr(lst->argv[i + 1], '\"'))
					lst->in_quote = 1;
			lst->delim[k] = ft_strdup(unquote(lst->argv[i + 1]));
			k++;
		}
		i++;
	}
	lst->delim[k] = NULL;
}

char	*creat_heroc(t_cmd *lst)
{
	int		i;
	char	*tmp;

	i = 0;
	while (++i)
	{
		tmp = ft_strjoin("heredoc", ft_itoa(i));
		if (!tmp)
			return (NULL);
		lst->fd = open(tmp, O_RDONLY, 0644);
		if (lst->fd == -1)
		{
			lst->fd = open(tmp, O_RDWR | O_CREAT | O_TRUNC, 0644);
			if (lst->fd < 0)
				return (NULL);
			return (tmp);
		}
	}
	return (NULL);
}

void	her_sin(int sig)
{
	if (sig == SIGINT)
	{
		close(0);
		ex_st(1, 1);
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
			return (1);
		}
		if (!tmp || ((ft_strncmp(tmp, delim, ft_strlen(delim)) == 0)
				&& (ft_strlen(tmp) == ft_strlen(delim))))
			break ;
		exp = expand_heredoc(tmp, env);
		if (exp && in != 1)
		{
			write(lst->fd, exp, ft_strlen(exp));
			write(lst->fd, "\n", 1);
			free(exp);
		}
		else
		{
			write(lst->fd, tmp, ft_strlen(tmp));
			write(lst->fd, "\n", 1);
			free(tmp);
		}
	}
	free(tmp);
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
				close(lst->fd);
				unlink(tmp);
				break ;
			}
			if (lst->infile != 0)
				close(lst->infile);
			lst->infile = open(tmp, O_RDONLY);
			close(lst->fd);
			unlink(tmp);
		}
		lst = lst->next;
	}
}

int	is_heredoc(t_cmd *lst)
{
	int	i;
	int	res;

	res = 0;
	while (lst)
	{
		i = 0;
		lst->is_heredoc = 0;
		while (lst->argv[i] && lst->argv[i + 1])
		{
			if (!ft_strcmp(lst->argv[i], "<<") && ft_strcmp(lst->argv[i + 1], "<"))
			{
				lst->is_heredoc = 1;
				res = 1;
			}
			i++;
		}
		lst = lst->next;
	}
	return (res);
}
