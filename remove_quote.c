#include "minishell.h"

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
