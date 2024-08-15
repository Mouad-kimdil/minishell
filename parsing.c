#include "minishell.h"

void	my_free(void *ptr)
{
	if (ptr)
	{
		free(ptr);
		ptr = NULL;
	}
}

char	*get_env_value_2(char *name, t_env *env)
{
	while (env)
	{
		if (ft_strcmp(name, env->name) == 0)
			return (env->value);
		env = env->next;
	}
	return ("");
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
			dolar_dolar_case(&exp, &j);
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
			return (fr(res), put_fd("Minishell: syntax error\n", 2), 1);
		if (res[0][0] == '|')
			return (fr(res), put_fd("Minishell: syntax error\n", 2), 1);
		if (is_red(res[i][0]) && syn_err_chars(res[i + 1][0]))
			return (fr(res), put_fd("Minishell: syntax error\n", 2), 1);
		if (res[i][0] == '|' && !res[i + 1])
			return (fr(res), put_fd("Minishell: synatx error\n", 2), 1);
		i++;
	}
	return (fr(res), 0);
}
