/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 14:55:09 by mkimdil           #+#    #+#             */
/*   Updated: 2024/08/14 05:15:02 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_del(t_cmd *lst)
{
	int		i;
	int		k;
	int		del_size;
	char	*tmp1;

	del_size = get_del_size(lst) * 2;
	lst->del = malloc(sizeof(char *) * (del_size + 1));
	if (!lst->del)
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
			lst->del[k] = ft_strdup(tmp1);
			free(tmp1);
			k++;
		}
	}
	lst->del[k] = NULL;
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
		tmp = ft_strjoin("/tmp/.heredoc", tmp1);
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

void	perferm_heredoc_help(int fd, char *exp)
{
	if (exp)
	{
		write(fd, exp, ft_strlen(exp));
		write(fd, "\n", 1);
		free(exp);
	}
}

int	perferm_heredoc(t_cmd *lst, char *del, t_list *env)
{
	char	*exp;
	char	*tmp;
	(void)	*env;
	while (1)
	{
		signal(SIGINT, her_sin);
		tmp = readline("> ");
		if (!ttyname(0))
		{
			close(lst->inf);
			open(ttyname(2), O_RDWR);
			return (free(tmp), 1);
		}
		if (!tmp || ((ft_strncmp(tmp, del, ft_strlen(del)) == 0)
				&& (ft_strlen(tmp) == ft_strlen(del))))
		{
			free(tmp);
			break ;
		}
		exp = expand_heredoc(tmp, env, lst->in_quote);
		free(tmp);
		perferm_heredoc_help(lst->fd, exp);
	}
	return (0);
}

int	heredoc(t_cmd *l, t_list *env)
{
	char	*t;
	int		i;
	int		fd;

	while (l)
	{
		(1) && (i = -1, get_del(l), 0);
		while (l->del[++i])
		{
			(1) && (t = creat_heroc(l), fd = open(t, O_RDONLY), 0);
			unlink(t);
			if (perferm_heredoc(l, l->del[i], env))
			{
				(1) && (fr(l->del), free(t), close(l->fd), close(fd), 0);
				return (1);
			}
			(l->inf != 0) && (close(l->inf), 0);
			(1) && (l->inf = fd, close(l->fd), free(t), 0);
		}
		fr(l->del);
		l = l->next;
	}
	return (0);
}
