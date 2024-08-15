#include "minishell.h"

int	arglen(char **arg)
{
	int	i;

	i = 0;
	while (arg[i])
		i++;
	return (i);
}

int	count_single(char *input)
{
	int		count;
	int		i;
	char	quote;

	i = -1;
	count = 0;
	quote = '\'';
	i = -1;
	while (input[++i])
	{
		if (input[i] == quote)
		{
			count++;
			continue ;
		}
	}
	return (count);
}

int	count_double(char *input)
{
	int		count;
	int		i;
	char	quote;

	i = -1;
	count = 0;
	quote = '\"';
	i = -1;
	while (input[++i])
	{
		if (input[i] == quote)
		{
			count++;
			continue ;
		}
	}
	return (count);
}

char	*unquote(char	*input)
{
	t_expand	unq;
	int			i;
	char		*result;
	char		*p;

	unq.len = ft_strlen(input);
	result = malloc(unq.len + 1);
	if (!result)
		return (NULL);
	p = result;
	unq.in_single_quote = 0;
	unq.in_double_quote = 0;
	i = -1;
	while (++i < unq.len)
	{
		if (input[i] == '\'' && !unq.in_double_quote)
			unq.in_single_quote = !unq.in_single_quote;
		else if (input[i] == '"' && !unq.in_single_quote)
			unq.in_double_quote = !unq.in_double_quote;
		else
			*p++ = input[i];
	}
	*p = '\0';
	return (result);
}
