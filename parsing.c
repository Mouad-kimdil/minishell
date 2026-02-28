#include "minishell.h"

int	double_red(char *s)
{
	if (!ft_strcmp(s, "<<") || !ft_strcmp(s, ">>"))
		return (1);
	return (0);
}

int	is_red(int c)
{
	if (c == '>' || c == '<')
		return (1);
	return (0);
}

int	last_check(int c)
{
	if (c == '`' || c == '(' || c == ')')
		return (1);
	return (0);
}

int	syn_err_chars(int c)
{
	if (c == '<' || c == '>')
		return (1);
	return (0);
}

int	check_line(char **res)
{
	int	i;
	int	j;
	int	tr;

	i = 0;
	tr = 0;
	while (res[i])
	{
		j = 0;
		tr = 0;
		while (res[i][j])
		{
			if (res[i][j] == '<')
				tr++;
			j++;
		}
		if (tr > 2)
			return (1);
		i++;
	}
	return (0);
}

int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	count_num_of_special(char *line)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (line[i])
	{
		if ((line[i] == '<' && line[i + 1] == '<')
			|| (line[i] == '>' && line[i + 1] == '>'))
			count++, i += 2;
		else if (line[i] == '|' && line[i + 1] == '|')
			count++, i += 2;
		else if (line[i] == '&' && line[i + 1] == '&')
			count++, i += 2;
		else if (is_special_char(line[i]))
			count++, i++;
		else
			i++;
	}
	return (count);
}

void	second_case(char *str, char *line, int *i, int *j)
{
	str[(*j)++] = ' ';
	str[(*j)++] = line[(*i)++];
	str[(*j)++] = line[(*i)++];
	str[(*j)++] = ' ';
}

char	*add_space(char *line)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	j = 0;
	str = malloc(ft_strlen(line) + count_num_of_special(line) * 2 + 1);
	if (!str)
		return (NULL);
	while (line[i])
	{
		if ((line[i] == '<' && line[i + 1] == '<')
			|| (line[i] == '>' && line[i + 1] == '>'))
			second_case(str, line, &i, &j);
		else if (line[i] == '|' && line[i + 1] == '|')
		{
			str[j++] = ' ', str[j++] = line[i++], str[j++] = line[i++], str[j++] = ' ';
		}
		else if (line[i] == '&' && line[i + 1] == '&')
		{
			str[j++] = ' ', str[j++] = line[i++], str[j++] = line[i++], str[j++] = ' ';
		}
		else if (is_special_char(line[i]))
		{
			str[j++] = ' ';
			str[j++] = line[i++];
			str[j++] = ' ';
		}
		else
			str[j++] = line[i++];
	}
	str[j] = '\0';
	return (str);
}

void	my_free(void *ptr)
{
	if (ptr)
	{
		free(ptr);
		ptr = NULL;
	}
}

char	*expand_export(t_cmd *lst, t_list *envp, int i)
{
	t_expand	exp;
	int			j;

	exp.cmd = NULL;
	exp.current = lst->argv[i];
	j = 0;
	if (ft_strnstr(exp.current, "=\'"))
		exp.tr = 1;
	while (exp.current[j])
	{
		if (exp.current[j] == '$' && exp.current[j + 1] == '?')
			j++;
		else if (exp.current[j] == '$' && expand_cases(exp.current[j + 1])
			&& exp.tr != 1)
			handle_special_case(&exp, &j, lst, envp);
		else if (exp.current[j] == '$' && exp.current[j + 1] == '$')
			dollar_dollar_case(&exp, &j);
		else
			last_case(&exp, &j);
	}
	return (exp.cmd);
}

int	syn_error(char *line)
{
	char	**res;
	int		i;

	res = ft_split_2(line);
	if (!res || !*res)
		return (1);
	i = 0;
	while (res[i])
	{
		if ((res[i][0] == '>' || res[i][0] == '<'
			|| double_red(res[i]))
			&& (!res[i + 1] || syn_err_chars(res[i + 1][0])))
			return (free_str_array(res), put_fd("Minishell: syntax error\n", 2), 1);
		if (res[0][0] == '|')
			return (free_str_array(res), put_fd("Minishell: syntax error\n", 2), 1);
		if (is_red(res[i][0]) && res[i + 1] && syn_err_chars(res[i + 1][0]))
			return (free_str_array(res), put_fd("Minishell: syntax error\n", 2), 1);
		if (res[i][0] == '|' && !res[i + 1])
			return (free_str_array(res), put_fd("Minishell: syntax error\n", 2), 1);
		i++;
	}
	return (free_str_array(res), 0);
}
