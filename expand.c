#include "minishell.h"

t_exp	*ft_new_node(char *str)
{
	t_exp	*new;

	new = malloc(sizeof(t_exp));
	if (new)
	{
		new->splited = ft_split_2(str);
		new->next = NULL;
	}
	return (new);
}

t_exp	*last_node(t_exp *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

void	ft_add_back(t_exp **lst, t_exp *new)
{
	t_exp	*last;

	if (*lst == NULL)
		*lst = new;
	else
	{
		last = last_node(*lst);
		last->next = new;
	}
}

int	check_is_ambg(char *expanded)
{
	int	nb;

	nb = ft_strnstr(expanded, ">>");
	return (ft_strsearch(expanded, '>') || nb);
}

void	noexpand_single_quote(t_expand *exp, int *j)
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

void	dollar_dollar_case(t_expand *exp, int *j)
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
			free_str_array(ap.temp);
		}
		else
		{
			res[ap.k] = ft_strdup(argv[ap.i]);
			ap.k++;
		}
	}
	return (res[ap.k] = NULL, free_str_array(argv), res);
}

void	handle_single_quote(t_expand *exp, int *j)
{
	(*j)++;
	while (exp->current[*j] && exp->current[*j] != '\'')
		noexpand_single_quote(exp, j);
	(*j)++;
}

void	handle_double_quote(t_expand *exp, int *j, t_cmd *lst, t_list *envp)
{
	char	*temp;
	int		k;

	if (exp->current[*j] == '"')
		(*j)++;
	temp = NULL;
	if (exp->current[*j] == '$' && special_case(exp->current[*j + 1]))
	{
		(*j)++;
		k = *j;
		while (exp->current[*j] && special_case(exp->current[*j]))
			(*j)++;
		exp->name = ft_substr(exp->current, k, *j - k);
		exp->value = get_env_value(exp->name, envp->envs);
		temp = exp->cmd;
		free(exp->name);
		exp->cmd = ft_strjoin(temp, exp->value);
		free(temp);
	}
	else if (exp->current[*j] == '$' && is_number(exp->current[*j + 1]))
	{
		if (exp->current[*j + 1])
			(*j)++;
		numeric_expand(lst, exp, j);
	}
	if (exp->current[*j] == '"')
		(*j)++;
}

void	numeric_expand(t_cmd *lst, t_expand *exp, int *j)
{
	char	*temp1;

	(*j)++;
	lst->ambiguous = 1;
	temp1 = exp->cmd;
	exp->cmd = ft_strjoin(exp->cmd, NULL);
	free(temp1);
}

void	handle_special_case(t_expand *exp, int *j, t_cmd *lst, t_list *envp)
{
	char	*temp;
	int		k;

	temp = NULL;
	(*j)++;
	if (exp->current[*j] && is_number(exp->current[*j]))
		numeric_expand(lst, exp, j);
	else
	{
		k = *j;
		while (exp->current[*j] && special_case(exp->current[*j]))
			(*j)++;
		exp->name = ft_substr(exp->current, k, *j - k);
		exp->value = get_env_value(exp->name, envp->envs);
		free(exp->name);
		temp = exp->cmd;
		exp->cmd = ft_strjoin(temp, exp->value);
		free(temp);
	}
}

void	ret_val(t_expand *exp, int *j)
{
	char	*temp;
	char	*temp1;

	temp = exp->cmd;
	(*j) += 2;
	temp1 = ft_itoa(exit_status(0, 0));
	exp->cmd = ft_strjoin(exp->cmd, temp1);
	free(temp);
	free(temp1);
}

char	*expand_cmd(t_cmd *lst, t_list *envp, int i)
{
	t_expand	exp;
	int			j;

	exp.cmd = NULL;
	exp.current = lst->argv[i];
	j = 0;
	if (lst->expand_state == 1)
		return (expand_export(lst, envp, i));
	while (exp.current[j])
	{
		if (exp.current[j] == '$' && exp.current[j + 1] == '?')
			ret_val(&exp, &j);
		else if (exp.current[j] == '\'')
			handle_single_quote(&exp, &j);
		else if (exp.current[j] == '"')
			handle_double_quote(&exp, &j, lst, envp);
		else if (exp.current[j] == '$' && expand_cases(exp.current[j + 1]))
			handle_special_case(&exp, &j, lst, envp);
		else if (exp.current[j] == '$' && exp.current[j + 1] == '$')
			dollar_dollar_case(&exp, &j);
		else if (exp.current[j] == '$' && exp.current[j + 1] == '"')
			j++;
		else
			last_case(&exp, &j);
	}
	return (exp.cmd);
}

void	expand_with_space(t_cmd *lst, char *expanded, int *i)
{
	if (check_is_ambg(expanded))
		lst->expand_state = 1;
	else
		lst->ambiguous = 1;
	free(lst->argv[*i]);
	lst->argv[*i] = ft_strdup(expanded);
}

void	expand_without_space(t_cmd *lst, int *tr, int *i, char *expanded)
{
	if (lst->argv[*i] && expanded)
	{
		free(lst->argv[*i]);
		lst->argv[*i] = ft_strdup(expanded);
	}
	if (lst->argv[*i] && !expanded)
	{
		if (*tr != 1 && *tr != 2)
			lst->ambiguous = 1;
		free(lst->argv[*i]);
		lst->argv[*i] = NULL;
	}
}

void	check_expanded(t_cmd *lst, int *i, int *tr)
{
	if (!ft_strcmp(lst->argv[0], "export"))
		lst->expand_state = 1;
	if (lst->argv[*i] && ft_strsearch(lst->argv[*i], '"'))
	{
		*tr = 1;
		lst->in_quote = 2;
	}
	if (lst->argv[*i] && ft_strsearch(lst->argv[*i], '\''))
	{
		lst->in_quote = 2;
		*tr = 2;
	}
}

void	expand_helper(t_cmd *lst, t_list *envp, int *i, int *tr)
{
	char	*ex;

	ex = NULL;
	check_expanded(lst, i, tr);
	ex = expand_cmd(lst, envp, *i);
	if (*tr == 1 || *tr == 0)
	{
		if (ex && ((ft_strchr(ex, ' ') || ft_strchr(ex, '\t'))) && *tr == 0)
		{
			lst->expand_state = 0;
			expand_with_space(lst, ex, i);
		}
		else
		{
			lst->expand_state = 2;
			expand_without_space(lst, tr, i, ex);
		}
	}
	if (*tr == 2)
	{
		if (ft_strnstr(lst->argv[*i], "$'"))
		{
			free(lst->argv[*i]);
			lst->argv[*i] = ft_strdup(ex + 1);
		}
		else
		{
			free(lst->argv[*i]);
			lst->argv[*i] = ft_strdup(ex);
		}
	}
	free(ex);
}

void	print_av(char **av)
{
	for (int i = 0; av[i]; i++)
		printf("av[%d]: %s\n", i, av[i]);
}

void	expand(t_cmd *lst, t_list *envp)
{
	int		i;
	int		tr;

	tr = 0;
	while (lst)
	{
		lst->expand_state = -1;
		i = 0;
		while (lst->argv[i])
		{
			if (lst->is_heredoc)
				break ;
			if (ft_strchr(lst->argv[i], '$'))
				expand_helper(lst, envp, &i, &tr);
			i++;
		}
		if (lst->is_heredoc != 1)
			remove_null_values(lst->argv, i);
		if (lst->expand_state == 0)
			lst->argv = append(lst->argv, i);
		lst = lst->next;
	}
}
