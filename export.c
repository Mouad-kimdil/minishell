#include "minishell.h"

int	checking(t_env *tmp, char *name, char *value)
{
	while (tmp)
	{
		if (!ft_strcmp(tmp->name, name))
		{
			if (value != NULL)
			{
				free(tmp->value);
				tmp->value = ft_strdup(value);
			}
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

void	add_env(t_env **env, char *name, char *value)
{
	t_env	*new_node;
	t_env	*tmp;

	tmp = *env;
	if (checking(tmp, name, value))
		return ;
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->name = ft_strdup(name);
	if (!value)
		new_node->value = NULL;
	else
		new_node->value = ft_strdup(value);
	new_node->index = 0;
	new_node->next = NULL;
	if (!*env)
	{
		*env = new_node;
		return ;
	}
	tmp = *env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
}

int	add_the_value(char *name, char *value, t_list *list)
{
	t_env	*env;

	env = list->envs;
	if (!value)
		return (1);
	while (env)
	{
		if (!ft_strcmp(env->name, name))
		{
			free(env->value);
			env->value = ft_strdup(value);
			return (0);
		}
		env = env->next;
	}
	return (1);
}

int	update_the_value(char *name, char *value, t_list *list)
{
	t_env	*env;
	char	*temp;

	env = list->envs;
	if (!value)
		return (0);
	while (env)
	{
		if (!ft_strcmp(env->name, name))
		{
			temp = ft_strjoin(env->value, value);
			if (!temp)
				return (1);
			free(env->value);
			env->value = temp;
			return (0);
		}
		env = env->next;
	}
	return (1);
}

void	last_case(t_expand *exp, int *j)
{
	char	*temp1;

	temp1 = exp->cmd;
	exp->temp = ft_substr(exp->current, *j, 1);
	exp->cmd = ft_strjoin(temp1, exp->temp);
	free(temp1);
	free(exp->temp);
	(*j)++;
}

void	swap_the_nodes(t_env *tmp1, t_env *tmp2)
{
	char	*temp_name;
	char	*temp_value;
	int		temp_index;

	temp_name = tmp1->name;
	temp_value = tmp1->value;
	temp_index = tmp1->index;
	tmp1->name = tmp2->name;
	tmp1->value = tmp2->value;
	tmp1->index = tmp2->index;
	tmp2->name = temp_name;
	tmp2->value = temp_value;
	tmp2->index = temp_index;
}

void	sort_env(t_env *env)
{
	t_env	*tmp1;
	t_env	*tmp2;

	tmp1 = env;
	while (tmp1)
	{
		tmp2 = tmp1->next;
		while (tmp2)
		{
			if (ft_strcmp(tmp1->name, tmp2->name) > 0)
				swap_the_nodes(tmp1, tmp2);
			tmp2 = tmp2->next;
		}
		tmp1 = tmp1->next;
	}
}

void	index_env(t_env *env)
{
	t_env	*tmp;
	t_env	*tmp2;

	tmp = env;
	while (tmp)
	{
		tmp->index = 0;
		tmp = tmp->next;
	}
	tmp = env;
	while (tmp)
	{
		tmp2 = env;
		while (tmp2)
		{
			if (ft_strcmp(tmp->name, tmp2->name) > 0)
				tmp->index++;
			tmp2 = tmp2->next;
		}
		tmp = tmp->next;
	}
	sort_env(env);
}

char	*get_name(char *str)
{
	int		i;
	int		j;
	char	*result;

	i = 0;
	j = 0;
	while (str[i])
	{
		if ((str[i] == '=') || (str[i] == '+' && str[i + 1] == '='))
			break ;
		i++;
	}
	result = malloc(i + 1);
	if (!result)
		return (NULL);
	while (j < i)
	{
		result[j] = str[j];
		j++;
	}
	result[i] = '\0';
	return (result);
}

char	*get_value(char *str)
{
	char	*find;

	find = ft_strchr(str, '=');
	if (!find)
		return (NULL);
	return (find + 1);
}

void	print_export(t_env *env)
{
	int		size;
	int		i;

	i = 0;
	size = env_size(env);
	index_env(env);
	while (i < size)
	{
		if (env && env->index == i)
		{
			write(1, "declare -x ", 11);
			write(1, env->name, ft_strlen(env->name));
			if (env->value)
			{
				write(1, "=\"", 2);
				write(1, env->value, ft_strlen(env->value));
				write(1, "\"", 1);
			}
			write(1, "\n", 1);
			i++;
		}
		env = env->next;
	}
}

int	is_valid_to_export(char *str)
{
	int	i;

	i = 0;
	if (!is_lower(*str) && !is_upper(*str) && *str != '_')
		return (-1);
	while (str[i])
	{
		if (str[i] == '=')
			return (i + 1);
		if (str[i] == '+' && str[i + 1] == '=')
			return (i + 2);
		if (!(is_upper(str[i]) || is_lower(str[i])
				|| is_number(str[i]) || str[i] == '_'))
			return (-1);
		i++;
	}
	return (0);
}

void	checking_and_add(int is_valid, char *argv, t_list *list)
{
	char	*name;
	char	*value;
	int		is_modified;

	name = get_name(argv);
	value = get_value(argv);
	is_valid = ft_strlen(name);
	if (!(*argv + is_valid))
		is_modified = 1;
	else if (*(argv + is_valid) == '+')
		is_modified = update_the_value(name, value, list);
	else
		is_modified = add_the_value(name, value, list);
	if (is_modified == 1)
		add_env(&list->envs, name, value);
	free(name);
}

int	expand_cases(char c)
{
	return (is_ascii(c) || is_number(c) || c == '_');
}

void	export(char **argv, t_list *list)
{
	int		is_valid;
	t_env	*env;

	if (!*(argv + 1))
	{
		env = list->envs;
		print_export(env);
		return ;
	}
	argv++;
	while (*argv)
	{
		is_valid = is_valid_to_export(*argv);
		if (is_valid == -1)
		{
			printf("minishell: export: `%s': not a valid identifier\n", *argv);
			exit_status(1, 1);
		}
		else
		{
			checking_and_add(is_valid, *argv, list);
			exit_status(0, 1);
		}
		argv++;
	}
}
