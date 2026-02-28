#include "minishell.h"

void	heredoc_sigint(int sig)
{
	if (sig == SIGINT)
	{
		close(0);
		exit_status(1, 1);
	}
}

int	is_heredoc(t_cmd *lst)
{
	int	i;
	int	res;

	res = 0;
	while (lst)
	{
		i = 0;
		lst->is_heredoc = 0;
		while (lst->argv[i] && lst->argv[i + 1])
		{
			if (!ft_strcmp(lst->argv[i], "<<")
				&& ft_strcmp(lst->argv[i + 1], "<"))
			{
				lst->is_heredoc = 1;
				res = 1;
			}
			i++;
		}
		lst = lst->next;
	}
	return (res);
}

int	get_del_size(t_cmd *lst)
{
	int	count;
	int	i;

	count = 0;
	while (lst)
	{
		i = 0;
		while (lst->argv[i])
		{
			if (!ft_strcmp(lst->argv[i], "<<"))
				count++;
			i++;
		}
		lst = lst->next;
	}
	return (count);
}

int	ft_isspace(char str)
{
	while (str)
	{
		if (str == ' ' || str == '\t')
			return (1);
		str++;
	}
	return (0);
}

int	is_blank(char *str)
{
	while (*str)
	{
		if (!ft_isspace(*str))
			return (0);
		str++;
	}
	return (1);
}

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
			dollar_dollar_case(&exp, &j);
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

void	get_del(t_cmd *lst)
{
	int		i;
	int		k;
	int		del_size;
	char	*tmp1;

	del_size = get_del_size(lst) * 2;
	lst->del = malloc(sizeof(char *) * (del_size + 1));
	if (!lst->del)
		return ;
	i = -1;
	k = 0;
	while (lst->argv[++i] && lst->argv[i + 1])
	{
		if (!ft_strcmp(lst->argv[i], "<<"))
		{
			if (ft_strchr(lst->argv[i + 1], '\'')
				|| ft_strchr(lst->argv[i + 1], '\"'))
				lst->in_quote = 1;
			tmp1 = unquote(lst->argv[i + 1]);
			lst->del[k] = ft_strdup(tmp1);
			free(tmp1);
			k++;
		}
	}
	lst->del[k] = NULL;
}

char	*create_heredoc(t_cmd *lst)
{
	int		i;
	char	*tmp;
	char	*tmp1;

	i = 0;
	while (++i)
	{
		tmp1 = ft_itoa(i);
		tmp = ft_strjoin("/tmp/.heredoc", tmp1);
		if (!tmp || !tmp1)
			return (free(tmp), free(tmp1), NULL);
		free(tmp1);
		lst->fd = open(tmp, O_RDONLY, 0644);
		if (lst->fd == -1)
		{
			lst->fd = open(tmp, O_RDWR | O_CREAT | O_TRUNC, 0644);
			if (lst->fd < 0)
				return (free(tmp), free(tmp1), NULL);
			return (tmp);
		}
	}
	return (NULL);
}

void	perform_heredoc_help(int fd, char *exp)
{
	if (exp)
	{
		write(fd, exp, ft_strlen(exp));
		write(fd, "\n", 1);
		free(exp);
	}
}

int	perform_heredoc(t_cmd *lst, char *del, t_list *env)
{
	char	*exp;
	char	*tmp;

	(void)env;
	while (1)
	{
		signal(SIGINT, heredoc_sigint);
		tmp = readline("> ");
		if (!ttyname(0))
			return (free(tmp), 1);
		if (!tmp || ((ft_strncmp(tmp, del, ft_strlen(del)) == 0)
				&& (ft_strlen(tmp) == ft_strlen(del))))
		{
			free(tmp);
			break ;
		}
		exp = expand_heredoc(tmp, env, lst->in_quote);
		free(tmp);
		perform_heredoc_help(lst->fd, exp);
	}
	return (0);
}

int	heredoc(t_cmd *l, t_list *env)
{
	char	*t;
	int		i;
	int		fd;

	while (l)
	{
		i = -1;
		get_del(l);
		while (l->del[++i])
		{
			t = create_heredoc(l);
			fd = open(t, O_RDONLY);
			unlink(t);
			if (perform_heredoc(l, l->del[i], env))
				return (free_str_array(l->del), free(t), close(l->fd), close(fd), 1);
			(l->inf != 0) && (close(l->inf), 0);
			l->inf = fd;
			close(l->fd);
			free(t);
		}
		free_str_array(l->del);
		l = l->next;
	}
	return (0);
}
