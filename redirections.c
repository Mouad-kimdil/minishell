#include "minishell.h"

void	new_array(t_cmd *node, int *index, int j)
{
	free(node->argv[j]);
	free(node->argv[j + 1]);
	while (node->argv[j + 2])
	{
		node->argv[j] = node->argv[j + 2];
		j++;
	}
	node->argv[j] = NULL;
	*index -= 1;
}

int	is_rdr(char *arg)
{
	if (ft_strchr(arg, '>') || ft_strnstr(arg, ">>"))
		return (1);
	return (0);
}

int	checking_ambiguous(t_cmd *node)
{
	t_cmd	*tmp;
	int		o;

	o = 0;
	tmp = node;
	if (node->ambiguous == 1)
	{
		write(2, "ambiguous redirect\n", 19);
		while (tmp->argv[o])
		{
			free(tmp->argv[o]);
			tmp->argv[o] = ft_strdup("#");
			o++;
		}
		exit_status(1, 1);
		return (1);
	}
	return (0);
}

int	split_stlen(char **str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

int	checking_error(t_cmd *node, int index)
{
	char	**str;

	str = ft_split_2(node->argv[index]);
	if (split_stlen(str) > 1)
	{
		exit_status(1, 1);
		write(2, "ambiguous redirect\n", 19);
		return (1);
	}
	return (0);
}

void	apply_redirs_and_pipe_out(t_cmd *node, t_execute *exec)
{
	if (node->inf != 0)
	{
		if (dup2(node->inf, 0) == -1)
			msg_error("dup2 in inf");
		close(node->inf);
	}
	if (node->outfile != 1)
	{
		if (dup2(node->outfile, 1) == -1)
			msg_error("dup2 in outfile");
		close(node->outfile);
	}
	else
	{
		close(exec->fd[0]);
		if (dup2(exec->fd[1], 1) == -1)
			msg_error("dup2 in fd[1]");
		close(exec->fd[1]);
	}
}

void	apply_redirs(t_cmd *node)
{
	if (node->inf != 0)
	{
		if (dup2(node->inf, 0) == -1)
			msg_error("dup2 in inf");
		close(node->inf);
	}
	if (node->outfile != 1)
	{
		if (dup2(node->outfile, 1) == -1)
			msg_error("dup2 in outfile");
		close(node->outfile);
	}
}

int	redirect_out_append(t_cmd *node, int *index, int flags)
{
	int		j;
	char	*str;

	if (checking_ambiguous(node) == 1)
		return (1);
	if (node->argv[*index + 1])
	{
		if (node->outfile != 1)
			close(node->outfile);
		node->outfile = open(node->argv[*index + 1], flags, 0644);
		if (node->outfile == -1)
		{
			str = ft_strjoin("minishell: ", node->argv[*index + 1]);
			msg_error(str);
			return (free(str), exit_status(1, 1), 1);
		}
		j = *index;
		new_array(node, index, j);
	}
	else
	{
		write(2, "minishell: no such file or directory\n", 37);
		return (exit_status(1, 1), 1);
	}
	return (0);
}

int	redirect_in(t_cmd *node, int *index, int flags)
{
	int		j;
	char	*str;

	if (checking_ambiguous(node) == 1)
		return (1);
	if (node->argv[*index + 1])
	{
		if (node->inf != 0)
			close(node->inf);
		node->inf = open(node->argv[*index + 1], flags);
		if (node->inf == -1)
		{
			str = ft_strjoin("minishell: ", node->argv[*index + 1]);
			msg_error(str);
			return (free(str), exit_status(1, 1), 1);
		}
		j = *index;
		new_array(node, index, j);
	}
	else
	{
		write(2, "minishell: no such file or directory\n", 37);
		return (exit_status(1, 1), 1);
	}
	return (0);
}

int	redirect_out(t_cmd *node, int *index, int flags)
{
	int		j;
	char	*str;

	if (checking_ambiguous(node) == 1)
		return (1);
	if (node->argv[*index + 1])
	{
		if (node->outfile != 1)
			close(node->outfile);
		node->outfile = open(node->argv[*index + 1], flags, 0644);
		if (node->outfile == -1)
		{
			str = ft_strjoin("minishell: ", node->argv[*index + 1]);
			msg_error(str);
			return (free(str), exit_status(1, 1), 1);
		}
		j = *index;
		new_array(node, index, j);
	}
	else
	{
		write(2, "minishell: no such file or directory\n", 37);
		return (exit_status(1, 1), 1);
	}
	return (0);
}

int	check_for_redirection(t_cmd *node)
{
	int	i;
	int	fail;

	i = 0;
	fail = 0;
	while (node->argv[i])
	{
		if (!ft_strcmp(node->argv[i], ">"))
			fail = redirect_out(node, &i, O_RDWR | O_CREAT | O_TRUNC);
		else if (!ft_strcmp(node->argv[i], ">>"))
			fail = redirect_out_append(node, &i, O_WRONLY | O_CREAT | O_APPEND);
		else if (!ft_strcmp(node->argv[i], "<"))
			fail = redirect_in(node, &i, O_RDONLY);
		else if (!ft_strcmp(node->argv[i], "<<"))
			new_array(node, &i, i);
		if (fail == 1 || fail == 2)
			return (fail);
		i++;
	}
	return (0);
}
