#include "minishell.h"

void	noex_single(t_expand *exp, int *j)
{
	char	*temp;
	char	*temp1;

	while (exp->current[*j] && exp->current[*j] != '\'')
	{
		temp = ft_substr(exp->current, *j, 1);
		temp1 = exp->cmd;
		exp->cmd = ft_strjoin(temp1, temp);
		free(temp);
		free(temp1);
		(*j)++;
	}
}

void	dolar_dolar_case(t_expand *exp, int *j)
{
	char	*temp;

	temp = exp->cmd;
	exp->cmd = ft_strjoin(temp, NULL);
	if (temp)
		free(temp);
	(*j) += 2;
}

void	remove_null_values(char **argv, int size)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < size)
	{
		if (argv[i] != NULL)
			argv[j++] = argv[i];
		i++;
	}
	argv[j] = NULL;
}

char	**append(char **argv, int len)
{
	t_append	ap;
	char		**res;

	ap.total_len = len;
	ap.i = -1;
	while (++ap.i < len)
		if (ft_strchr(argv[ap.i], ' ') || ft_strchr(argv[ap.i], '\t'))
			ap.total_len += countword_2(argv[ap.i]);
	res = malloc(sizeof(char *) * (ap.total_len + 1));
	if (!res)
		return (argv);
	ap.i = -1;
	ap.k = 0;
	while (argv[++ap.i])
	{
		if (ft_strchr(argv[ap.i], ' ') || ft_strchr(argv[ap.i], '\t'))
		{
			ap.temp = ft_split_2(argv[ap.i]);
			ap.j = -1;
			while (ap.temp[++ap.j])
			{
				res[ap.k] = ft_strdup(ap.temp[ap.j]);
				ap.k++;
			}
			fr(ap.temp);
		}
		else
		{
			res[ap.k] = ft_strdup(argv[ap.i]);
			ap.k++;
		}
	}
	return (res[ap.k] = NULL, fr(argv), res);
}
