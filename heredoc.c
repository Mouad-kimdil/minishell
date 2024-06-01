/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 14:55:09 by mkimdil           #+#    #+#             */
/*   Updated: 2024/06/01 17:28:36 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	print_args(t_cmd *lst)
// {
// 	int	i;

// 	while (lst)
// 	{
// 		i = 0;
// 		while (lst->argv[i])
// 		{
// 			printf("lst->argv[%d] : %s\n", i, lst->argv[i]);
// 			printf("lst->cmd[%d] : %s\n", i, lst->cmd);
// 			i++;
// 		}
// 		lst = lst->next;
// 	}
// }

void	set_delim(t_cmd *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		i = 0;
		while (lst->argv[i])
		{
			if (lst->argv[i + 1] && ft_strcmp(lst->argv[i], "<<") == 0)
				lst->delimiter = ft_strdup(lst->argv[i + 1]);
			i++;
		}
		lst = lst->next;
	}
}

// void	print_delim(t_cmd *lst)
// {
// 	while (lst)
// 	{
// 		if (lst->delimiter)
// 			printf("delim: %s\n", lst->delimiter);
// 		lst = lst->next;
// 	}
// }

int	num_of_her(t_cmd *lst)
{
	int	count;

	count = 0;
	while (lst)
	{
		if (lst->delimiter)
			count++;
		lst = lst->next;
	}
	return (count);
}

void	multipple_her(t_cmd **lst)
{
	int		i;
	int		count;
	char	*tmp;

	count = num_of_her(*lst);
	i = 0;
	while (lst && *lst && (*lst)->delimiter == NULL)
		(*lst) = (*lst)->next;
	printf("%s\n", (*lst)->delimiter);
	while (i < count - 1)
	{
		tmp = readline("> ");
		if (!tmp || ((ft_strncmp(tmp, (*lst)->delimiter, ft_strlen((*lst)->delimiter)) == 0)
				&& (ft_strlen(tmp) == ft_strlen((*lst)->delimiter))))
		{
			(*lst) = (*lst)->next;
			i++;		
			continue ;
		}
		free(tmp);
	}
}



int	here_doc(t_cmd *lst)
{
	char	*tmp;
	int		fd;

	set_delim(lst);
	multipple_her(&lst);
	fd = open("heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	while (1)
	{
		tmp = readline("> ");
		if (!tmp || ((ft_strncmp(tmp, lst->delimiter, ft_strlen(lst->delimiter)) == 0)
				&& (ft_strlen(tmp) == ft_strlen(lst->delimiter))))
			break ;
		write(fd, tmp, ft_strlen(tmp));
		write(fd, "\n", 1);
		free(tmp);
	}
	if (lst->delimiter[0] == '\n')
		free(lst->delimiter);
	free(tmp);
	close(fd);
	lst->infile = open("heredoc", O_RDONLY);
	unlink("heredoc");
	if (ft_strcmp(lst->argv[0], "<<") == 0)
		return (1);
	if (lst->infile < 0)
		exit(1);
	return (0);
}

int	is_heredoc(t_cmd *lst)
{
	int	i;

	while (lst)
	{
		i = 0;
		lst->is_heredoc = 0;
		lst->delimiter = NULL;
		while (lst->argv[i])
		{
			if (ft_strcmp(lst->argv[i], "<<") == 0)
				return (1);
			i++;
		}
		lst = lst->next;
	}
	return (0);
}
