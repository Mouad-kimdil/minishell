#include "minishell.h"

int	handle_single_double(char *line)
{
	int	i;
	int	tr;

	i = 0;
	tr = 0;
	while (line[i])
	{
		if (line[i] == '\'' && tr == 0)
			tr = 2;
		else if (line[i] == '\'' && tr == 2)
			tr = 0;
		else if (line[i] == '"' && tr == 0)
			tr = 1;
		else if (line[i] == '"' && tr == 1)
			tr = 0;
		i++;
	}
	if (tr == 1 || tr == 2)
		return (exit_status(258, 1), printf("syntax error\n"), 1);
	return (0);
}

void	mask_quoted_chars(char *line)
{
	int		i;
	int		tr;
	char	curr_quote;

	i = 0;
	tr = 0;
	curr_quote = '\0';
	while (line[i])
	{
		if ((line[i] == '\'' || line[i] == '"') && tr == 0)
		{
			tr = 1;
			curr_quote = line[i];
		}
		else if (line[i] == curr_quote && tr == 1)
		{
			tr = 0;
			curr_quote = '\0';
		}
		else if (tr == 1)
			line[i] = line[i] * -1;
		i++;
	}
}

void	back_to_ascii(t_cmd *lst)
{
	int	i;

	while (lst)
	{
		mask_quoted_chars(lst->cmd);
		i = 0;
		while (lst->argv[i])
		{
			mask_quoted_chars(lst->argv[i]);
			i++;
		}
		lst = lst->next;
	}
}

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

void	remove_quotes_from_arg_helper(char *arg, char quote_char, int *tr)
{
	int	i;
	int	j;

	i = -1;
	j = 0;
	while (++i < ft_strlen(arg))
	{
		if (arg[i] == quote_char && (*tr) == 1)
		{
			(*tr) = 0;
			continue ;
		}
		if (arg[i] != quote_char)
			arg[j++] = arg[i];
	}
	arg[j] = '\0';
}

void	remove_quotes_from_arg(char *arg, int in)
{
	int		i;
	int		tr;
	char	quote_char;

	if ((count_single(arg) == 1 && count_double(arg) == 0)
		&& (in == 1 || in == 2))
		return ;
	i = -1;
	tr = 0;
	quote_char = '\0';
	while (arg[++i])
	{
		if (arg[i] == '\'' || arg[i] == '\"')
		{
			quote_char = arg[i];
			tr = 1;
			break ;
		}
	}
	remove_quotes_from_arg_helper(arg, quote_char, &tr);
}

void	process_argv(char **argv, int in)
{
	while (*argv != NULL)
	{
		remove_quotes_from_arg(*argv, in);
		argv++;
	}
}

void	remove_quotes(t_cmd **lst)
{
	t_cmd	*current;

	current = *lst;
	while (current != NULL)
	{
		process_argv(current->argv, current->in_quote);
		current = current->next;
	}
}
