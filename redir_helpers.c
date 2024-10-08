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

int	checking_ambigious(t_cmd *node)
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
