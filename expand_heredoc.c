#include "minishell.h"

char	*get_env_value(char *name, t_env *env)
{
	while (env)
	{
		if (ft_strcmp(name, env->name) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	here_quote(t_expand *exp, int *j, t_list *envp)
{
	char	*temp;
	char	*temp1;
	int		k;

	k = *j;
	while (exp->current[*j] && exp->current[*j] != '$')
		(*j)++;
	temp1 = exp->cmd;
	temp = ft_substr(exp->current, k, *j - k);
	exp->cmd = ft_strjoin(temp1, temp);
	my_free(temp);
	my_free(temp1);
	if (exp->current[*j] && exp->current[*j] == '$')
		(*j)++;
	k = *j;
	while (exp->current[*j] && special_case(exp->current[*j]))
		(*j)++;
	temp1 = exp->cmd;
	exp->name = ft_substr(exp->current, k, *j - k);
	exp->value = get_env_value(exp->name, envp->envs);
	my_free(exp->name);
	exp->cmd = ft_strjoin(temp1, exp->value);
	my_free(temp1);
}

void	here_numeric_expand(t_expand *exp, int *j)
{
	char	*temp1;

	(*j)++;
	temp1 = exp->cmd;
	exp->cmd = ft_strjoin(exp->cmd, NULL);
	free(temp1);
}

void	here_special_case(t_expand *exp, int *j, t_list *envp)
{
	char	*temp;
	int		k;

	temp = NULL;
	(*j)++;
	if (exp->current[*j] && is_number(exp->current[*j]))
		here_numeric_expand(exp, j);
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

char	*expand_cmd_here(char *temp, t_list *envp)
{
	t_expand	exp;
	int			j;

	exp.cmd = NULL;
	exp.current = temp;
	j = 0;
	while (exp.current[j])
	{
		if (exp.current[j] == '\'' || exp.current[j] == '"')
			here_quote(&exp, &j, envp);
		else if (exp.current[j] == '$' && expand_cases(exp.current[j + 1]))
			here_special_case(&exp, &j, envp);
		else if (exp.current[j] == '$' && exp.current[j + 1] == '$')
			dolar_dolar_case(&exp, &j);
		else if (exp.current[j] == '$' && exp.current[j + 1] == '"')
			j++;
		else
			last_case(&exp, &j);
	}
	return (exp.cmd);
}

char	*expand_heredoc(char *temp, t_list *envp, int in)
{
	char	*expanded;

	expanded = NULL;
	if (ft_strchr(temp, '$') && in == -1)
		expanded = expand_cmd_here(temp, envp);
	if (!expanded)
		expanded = ft_strdup(temp);
	return (expanded);
}
